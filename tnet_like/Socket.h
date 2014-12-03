#ifndef SOCKET_H
#define SOCKET_H

#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <fcntl.h>

class Socket {
public:
	Socket(int fd = -1)
		: _fd(fd)
	{}

	int read(char *buf, int len)
	{
		int n = 0;
		do {
			n = ::read(_fd, buf, len);
		} while (-1 == n && EINTR == errno);
		return n;
	}
	
	int write(const char *buf, int len)
	{
		int n = 0;
		do {
			n = ::write(_fd, buf, len);
		} while (-1 == n && EINTR == errno);
		return n;
	}

	bool setNonblock()
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

private:
	int _fd;
};

#endif

