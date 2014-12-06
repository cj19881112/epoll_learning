#ifndef PACKET_H
#define PACKET_H

#include "Buffer.h"

class Packet {
public:
	virtual void encodeAndWrite(Buffer &outputBuffer) = 0;
};

#endif

