#ifndef TRANSPORT_H
#define TRANSPORT_H

#include "IoEvent.h"
#include "Poller.h"
#include "Acceptor.h"
#include "ChannelContext.h"

class Transport {
public:
	void run()
	{
		ChannelContext channelContext;
		std::vector<IOEvent> ioEvents;
		for ( ;; ) {
			ioEvents.clear();
			poller.poll(ioEvents);
			std::vector<IOEvent>::iterator it = ioEvents.begin();
			for (; it != ioEvents.end(); it++) {
				if (it->readOccurred) {
					it->channel->handleRead(channelContext);
				}
				if (it->writeOccurred) {
					it->channel->handleWrite(channelContext);
				}
				if (it->errorOccurred) {
					// TODO,remove channel from poller;
				}
			}
		}
	}
private:
	Acceptor acceptor;
	Poller poller;
};

#endif

