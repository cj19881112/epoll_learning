#ifndef CHANNEL_H
#define CHANNEL_H

#include "Buffer.h"
#include "Poller.h"
#include "Packet.h"
#include "ChannelContex.h"
#include "PacketStreamer.h"
#include "PacketHandler.h"

class Channel {
public:
	Channel(Poller *poller=0) 
		: _poller(poller)
	{
	}
	void handleRead(ChannelContext &context)
	{
		PacketStreamer *streamer = context.getPacketStreamer();
		PacketHandler *handler = context.getPacketHandler();
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
				handler->handlePacket(packet);
				streamer->deletePacket(packet);
			}
		} 

	}
	void handleWrite(ChannelContext &context)
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

	void postPacket(Packet &packet)
	{
		packet.encodeAndWrite(_outputBuffer);
		if (!_writeEnabled) {
			_poller->enableWrite(this);
		}
	}
	void setPoller(Poller *poller)
	{
		_poller = poller;
	}	
	Socket *getSocket()
	{
		return &_socket;
	}
private:
	Buffer _inputBuffer;
	Buffer _outputBuffer;
	Socket _socket;
	bool _writeEnabled;
	Poller *_poller;
};

#endif

