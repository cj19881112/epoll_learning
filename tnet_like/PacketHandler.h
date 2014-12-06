#ifndef PACKET_HANDLER_H
#define PACKET_HANDLER_H

#include "Packet.h"
#include "Channel.h"

class PacketHandler {
public:
	virtual void handlePacket(Packet *packet, Channel *channel) = 0;
};

#endif
