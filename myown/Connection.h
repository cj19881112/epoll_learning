#ifndef CONNECTION_H
#define CONNECTION_H

#include "Buffer.h"

class Connection {
public:
	void post(const std::string &msg);
	void post(const char *msg);
	void post(const char *byteBuf, int len);

	Buffer *readData();
	void writeData();

private:
	Buffer inputBuffer, outputBuffer;
};

#endif

