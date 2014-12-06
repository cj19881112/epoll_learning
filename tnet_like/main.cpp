#include "PacketStreamer.h"
#include "PacketHandler.h"
#include "Transport.h"
#include "Packet.h"
#include <string>

using namespace std;

class EchoPacket : public Packet {
public:
	EchoPacket(const char *str, int len) 
		: line(str, len)
	{
	}
	void encodeAndWrite(Buffer &outputBuffer) 
	{
		outputBuffer.fill(line.c_str(), line.size());
	}
private:
	string line;
};

class EchoPacketStreamer : public PacketStreamer {
public:
	Packet *newPacket(Buffer &inputBuffer)
	{
		int len = inputBuffer.available();
		char *p = inputBuffer.rawPtr();
		char *pNewLine = (char *)memchr(p, '\n', len);

		if (pNewLine) {
			EchoPacket *packet = new EchoPacket(p, pNewLine-p);
			inputBuffer.drop(pNewLine-p+1);
			return packet;
		}

		return 0;
	}
	void deletePacket(Packet *packet)
	{
		delete packet;
	}
};

class EchoPacketHandler : public PacketHandler {
public:
	void handlePacket(Packet *packet, Channel *channel)
	{
		channel->postPacket(*packet);
	}
};

int main()
{
	EchoPacketStreamer streamer;
	EchoPacketHandler handler;

	Transport transport(&streamer, &handler);
	transport.listen("0.0.0.0", 10000);
	transport.run();

	return 0;
}



