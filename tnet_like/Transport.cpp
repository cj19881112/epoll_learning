#include "Transport.h"
#include "IOEvent.h"
#include "ChannelContext.h"
#include "Channel.h"
#include "Poller.h"
#include "Acceptor.h"

#include <vector>

using std::vector;

Transport::Transport(PacketStreamer *streamer, PacketHandler *handler)
	: _streamer(streamer), _handler(handler), _poller(new Poller)
{
}

void Transport::listen(const char *host, int port)
{
	Acceptor *acceptor = new Acceptor(host, port);
	_poller->addChannel(acceptor);
}

void Transport::run()
{
	vector<IOEvent> ioEvents;

	for ( ;; ) {
		ioEvents.clear();
		_poller->poll(ioEvents);

		vector<IOEvent>::iterator it = ioEvents.begin();
		for (; it != ioEvents.end(); it++) {
			if (it->readOccurred) {
				it->channel->handleRead(_streamer, _handler, _poller);
			}
			if (it->wirteOccurred) {
				it->channel->handleWrite();
			}
			if (it->errorOccurred) {
				// TODO,remove channel from poller;
			}
		}
	}
}
