#ifndef SERVERSOCKET_H
#define SERVERSOCKET_H

#include "Socket.h"

class ServerSocket {
public:
	ServerSocket(const char *ip, int port);
	bool accept(Socket *socket);
	int getFd() { return _listenFd; }
private:
	int _listenFd;
};

#endif

