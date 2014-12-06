#ifndef TRANSPORT_H
#define TRANSPORT_H

class PacketStreamer;
class PacketHandler;
class Poller;

class Transport {
public:
	Transport(PacketStreamer *streamer, PacketHandler *handler);
	void run();
	void listen(const char *host, int port);
private:
	PacketStreamer *_streamer;
	PacketHandler *_handler;
	Poller *_poller;
};

#endif

