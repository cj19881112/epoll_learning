#include "Socket.h"
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <fcntl.h>

int Socket::read(char *buf, int len)
{
	int n = 0;
	do {
		n = ::read(_fd, buf, len);
	} while (-1 == n && EINTR == errno);
	return n;
}

int Socket::write(const char *buf, int len)
{
	int n = 0;
	do {
		n = ::write(_fd, buf, len);
	} while (-1 == n && EINTR == errno);
	return n;
}

bool Socket::setNonblock()
{
	int flag = fcntl(_fd, F_GETFL);

	if (flag == -1) {
		return false;
	}
	if (fcntl(_fd, F_SETFL, flag | O_NONBLOCK) == -1) {
		return false;
	}

	return true;
}

