#include "ServerSocket.h"
#include "Socket.h"

#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

ServerSocket::ServerSocket(const char *ip, int port)
{
	struct sockaddr_in addr;
	socklen_t addrLen;
	addrLen = sizeof (addr);

	memset(&addr, 0, addrLen);
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(ip);
	addr.sin_port = htons(port);

	_listenFd = socket(AF_INET, SOCK_STREAM, 0);
	assert(_listenFd > 0);

	int err = bind(_listenFd, (struct sockaddr*)&addr, addrLen);
	assert(-1 != err);

	err = listen(_listenFd, 64);
	assert(-1 != err);
}

bool ServerSocket::accept(Socket *socket)
{
	int cliFd = -1;
	do {
		cliFd = ::accept(_listenFd, NULL, NULL);
	} while (cliFd < 0 && EINTR == errno);
	if (cliFd < 0) {
		return false;
	}
	socket->setFd(cliFd);
	return true;
}
