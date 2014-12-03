#ifndef CHANNEL_H
#define CHANNEL_H

#include "Buffer.h"

class Channel {
public:
	Channel(Poller *poller) 
		: _poller(poller)
	{
	}
	void handleRead(ChannelContext &context)
	{
		PacketStreamer *streamer = context.getStreamer();
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
				packet = streamer.newPacket(_inputBuffer);
				if (!packet) {
					break;
				}
				handler.handlePacket(packet);
				streamer.deletePacket(packet);
			}
		} 

	}
	void handleWrite(ChannelContext &context)
	{
		
	}

	void postPacket(Packet &packet)
	{
		packet.encodeAndWrite(_outputBuffer);
		if (!_writeEnabled) {
		}
	}
private:
	Buffer _inputBuffer;
	Buffer _outputBuffer;
	Socket _socket;
	bool _writeEnabled;
	Poller *poller;
};

#endif

