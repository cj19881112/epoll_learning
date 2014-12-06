#ifndef SOCKET_H
#define SOCKET_H

class Socket {
public:
	Socket(int fd = -1)
		: _fd(fd)
	{}

	int read(char *buf, int len);
	int write(const char *buf, int len);
	bool setNonblock();

	int getFd()
	{
		return _fd;
	}

	void setFd(int fd)
	{
		_fd = fd;
	}

private:
	int _fd;
};

#endif

