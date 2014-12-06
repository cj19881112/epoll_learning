#ifndef PACKET_STREAMER_H
#define PACKET_STREAMER_H

#include "Buffer.h"

class Packet;

class PacketStreamer {
public:
	virtual Packet *newPacket(Buffer &inputBuffer) = 0;
	virtual void deletePacket(Packet *packet) = 0;
};

#endif

