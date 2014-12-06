#include "Acceptor.h"
#include "Poller.h"

#include <iostream>

using std::cout;
using std::endl;

Acceptor::Acceptor(const char *host, int port)
	: _host(host), _port(port), _serverSocket(host, port)
{

}

void Acceptor::handleRead(PacketStreamer *streamer, 
		PacketHandler *handler, Poller *poller)
{

	Channel *channel = new Channel;
	_serverSocket.accept(channel->getSocket());
	channel->getSocket()->setNonblock();
	poller->addChannel(channel);
	cout << "accept a client" << endl;
}

void Acceptor::handleWrite()
{
}

void Acceptor::postPacket(Packet &packet)
{
}

