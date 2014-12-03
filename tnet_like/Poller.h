#ifndef POLLER_H
#define POLLER_H

#include "IOEvent.h"

class Channel;
class Poller {
public:
	Poller()
	{
		// TODO: error checking
		_epollFd = epoll_create(1);
	}
	~Poller()
	{
		close(_epollFd);
	}
	void enableWrite(Channel *channel)
	{
		int fd = channels->getSocket()->getFd();
		if (channels.find(channel)) {
			struct epoll_event event;
			event.data.ptr = channel;
			event.events |= EPOLLOUT;
			epoll_ctl(_epollFd, EPOLL_CTL_MOD, fd, &event);
		}
	}
	void disableWrite(Channel *channel)
	{
		int fd = channels->getSocket()->getFd();
		if (channels.find(channel)) {
			struct epoll_event event;
			event.data.ptr = channel;
			event.events &= ~EPOLLOUT;
			epoll_ctl(_epollFd, EPOLL_CTL_MOD, fd, &event);
		}
	}
	void enableRead(Channel *channel)
	{
		int fd = channels->getSocket()->getFd();
		if (channels.find(channel)) {
			struct epoll_event event;
			event.data.ptr = channel;
			event.events |= EPOLLIN;
			epoll_ctl(_epollFd, EPOLL_CTL_MOD, fd, &event);
		}
	}
	void addChannel(Channel *channel)
	{
		channels.insert(channel);
	}
	void poll(std::vector<IOEvent> &ioEvents, int timeOut=-1)
	{
		struct epoll_event events[MAX_EVENT];
		int nevent = epoll_wait(_epollFd, events, MAX_EVENT, timeOut);
		// TODO: ERROR CHECKING
		for (int i = 0; i < nevent; i++) {
			IOEvent ioEvent;
			ioEvent.readOccurred = ((EPOLLIN & events[i].events) != 0);
			ioEvent.readOccurred = ((EPOLLOUT & events[i].events) != 0);
			if (EPOLLHUP&event[i].events || EPOLLERR&event[i].events) {
				ioEvent.errorOccurred = true;
			}
			ioEvent.channel = events[i].data.ptr;
			ioEvents.push(ioEvent);
		}
	}
private:
	std::set<Channel*> channels;
	int _epollFd;
};

#endif

