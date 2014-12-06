#include "Channel.h"
#include "PacketStreamer.h"
#include "PacketHandler.h"
#include "ChannelContext.h"
#include "Poller.h"

Channel::Channel(Poller *poller) 
	: _poller(poller)
{
}

void Channel::handleRead(PacketStreamer *streamer, 
		PacketHandler *handler, Poller *)
{
	Packet *packet = 0;
	int n = 0;
	const int maxReadTime = 20;

	for (int i = 0; i < maxReadTime; i++) {
		n = _inputBuffer.fill(_socket);
		if (n == 0) {
			break;
		}

		for ( ;; ) {
			packet = streamer->newPacket(_inputBuffer);
			if (!packet) {
				break;
			}
			handler->handlePacket(packet, this);
			streamer->deletePacket(packet);
		}
	} 

}
void  Channel::handleWrite()
{
	int n = 0;
	do {
		n = _outputBuffer.pour(_socket);
		if (0 == n) {
			break;
		}	
	} while (_outputBuffer.available() > 0);
	if (0 == _outputBuffer.available()) {
		_poller->disableWrite(this);
	}	
}

void  Channel::postPacket(Packet &packet)
{
	packet.encodeAndWrite(_outputBuffer);
	if (!_writeEnabled) {
		_poller->enableWrite(this);
	}
}
