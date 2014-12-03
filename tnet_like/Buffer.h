#ifndef BUFFER_H
#define BUFFER_H

#include "Socket.h"
#include <cstring>

class Buffer {
public:
	enum { LINE_SIZE = 8192 };
	int fill(Socket &socket)
	{
		ensureFreeSpace(LINE_SIZE);
		return socket.read(rawPtr(), LINE_SIZE);
	}

	int fill(const char *byteBuf, int len)
	{
		ensureFreeSpace(len);
		memmove(_dataEnd, byteBuf, len);
		_dataEnd += len;
	}

	int pour(Socket &socket)
	{
		int n = socket.write(rawPtr(), available());	
		drop(n);
		return n;
	}

	int pour(char *byteBuf, int len)
	{
		memcpy(byteBuf, rawPtr(), len);
		add(len);
		return len;
	}

	char *rawPtr()
	{
		return _dataBegin;
	}

	int available()
	{
		return _dataEnd - _dataBegin;	
	}

	void drop(int len)
	{
		_dataBegin += len;
	}

	void add(int len)
	{
		_dataEnd += len;
	}

private:
	int freeSpace()
	{
		return (_byteBufferEnd - _dataEnd) + (_dataBegin - _byteBufferBegin);
	}

	int capacity()
	{
		return _byteBufferEnd - _byteBufferBegin;
	}

	int tailFreeSpace()
	{
		return (_byteBufferEnd - _dataEnd);
	}

	void ensureFreeSpace(int len)
	{
		int dataSize = available();

		if (freeSpace() < len) {
			int newCapacity = capacity() * 2;

			char *newBuffer = new char[newCapacity];		
			memmove(newBuffer, _byteBufferBegin, dataSize);
			delete [] _byteBufferBegin;

			_dataBegin = _byteBufferBegin = newBuffer;
			_dataEnd = _dataBegin + dataSize;
			_byteBufferEnd = newBuffer + newCapacity;
		} else {
			if (tailFreeSpace() < len) {
				memmove(_byteBufferBegin, _dataBegin, dataSize);
				_dataBegin = _byteBufferBegin;
				_dataEnd = _dataBegin + dataSize;
			}
		}
	}

	char *_dataBegin;
	char *_dataEnd;
	char *_byteBufferBegin;
	char *_byteBufferEnd;
};

#endif

