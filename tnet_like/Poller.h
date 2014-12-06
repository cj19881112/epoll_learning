#ifndef POLLER_H
#define POLLER_H

#include "IOEvent.h"
#include <vector>
#include <set>

class Channel;
class Poller {
public:
	enum { MAX_EVENT = 256 };
	Poller();
	~Poller();
	void enableWrite(Channel *channel);
	void disableWrite(Channel *channel);
	void enableRead(Channel *channel);
	void addChannel(Channel *channel, IOEvent::EventType t=IOEvent::READ);
	void poll(std::vector<IOEvent> &ioEvents, int timeOut=-1);
private:
	std::set<Channel*> channels;
	int _epollFd;
};

#endif

