#ifndef CHANNEL_H
#define CHANNEL_H

#include "Buffer.h"
#include "Socket.h"

class Poller;
class ChannelContext;
class Packet;
class PacketStreamer;
class PacketHandler;

class Channel {
public:
	Channel(Poller *poller=0);
	virtual void handleRead(PacketStreamer *streamer, 
			PacketHandler *handler, Poller *poller);
	virtual void handleWrite();
	virtual void postPacket(Packet &packet);
	virtual int getFd() { return _socket.getFd(); }

	void setPoller(Poller *poller) { _poller = poller; }	
	Socket *getSocket() { return &_socket; }
private:
	Buffer _inputBuffer;
	Buffer _outputBuffer;
	Socket _socket;
	bool _writeEnabled;
	Poller *_poller;
};

#endif

