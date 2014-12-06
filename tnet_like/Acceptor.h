#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include <string>
#include "Channel.h"
#include "ServerSocket.h"

class Acceptor : public Channel {
public:
	Acceptor(const char *host, int port);
	virtual void handleRead(PacketStreamer *streamer, 
			PacketHandler *handler, Poller *poller);
	virtual void handleWrite();
	virtual void postPacket(Packet &packet);
	virtual int getFd() { return _serverSocket.getFd(); }
private:
	std::string _host;
	int _port;
	ServerSocket _serverSocket;
};

#endif

