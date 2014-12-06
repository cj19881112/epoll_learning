#include "Poller.h"
#include "Channel.h"
#include <sys/epoll.h>
#include <unistd.h>
#include <assert.h>
#include <cstdio>

Poller::Poller()
{
	// TODO: error checking
	_epollFd = epoll_create(1);
	assert(_epollFd > 0);
}
Poller::~Poller()
{
	close(_epollFd);
}
void Poller::enableWrite(Channel *channel)
{
	int fd = channel->getFd();
	if (channels.find(channel) != channels.end()) {
		struct epoll_event event;
		event.data.ptr = channel;
		event.events |= EPOLLOUT;
		epoll_ctl(_epollFd, EPOLL_CTL_MOD, fd, &event);
	}
}
void Poller::disableWrite(Channel *channel)
{
	int fd = channel->getFd();
	if (channels.find(channel) != channels.end()) {
		struct epoll_event event;
		event.data.ptr = channel;
		event.events = EPOLLIN;
		epoll_ctl(_epollFd, EPOLL_CTL_MOD, fd, &event);
	}
}
void Poller::enableRead(Channel *channel)
{
	int fd = channel->getFd();
	if (channels.find(channel) != channels.end()) {
		struct epoll_event event;
		event.data.ptr = channel;
		event.events |= EPOLLIN;
		epoll_ctl(_epollFd, EPOLL_CTL_MOD, fd, &event);
	}
}
void Poller::addChannel(Channel *channel, IOEvent::EventType type)
{
	channel->setPoller(this);
	struct epoll_event event;
	event.data.ptr = channel;
	int fd = channel->getFd();
	if (type == IOEvent::READ) {
		event.events = EPOLLIN;
	} else if (type == IOEvent::WRITE) {
		event.events = EPOLLOUT;
	} else if (type == IOEvent::READ_WRITE) {
		event.events = EPOLLIN|EPOLLOUT;
	} else {
		assert(0);
	}
	int err = epoll_ctl(_epollFd, EPOLL_CTL_ADD, fd, &event);
	assert(err == 0);
	channels.insert(channel);
}
void Poller::poll(std::vector<IOEvent> &ioEvents, int timeOut)
{
	struct epoll_event events[MAX_EVENT] = {0, };
	int nevent = epoll_wait(_epollFd, events, MAX_EVENT, timeOut);
	// TODO: ERROR CHECKING
	for (int i = 0; i < nevent; i++) {
		IOEvent ioEvent = {false, false, false, 0};
		ioEvent.readOccurred = ((EPOLLIN & events[i].events) != 0);
		ioEvent.wirteOccurred= ((EPOLLOUT & events[i].events) != 0);
		if (EPOLLHUP&events[i].events || EPOLLERR & events[i].events) {
			ioEvent.errorOccurred = true;
		}
		ioEvent.channel = static_cast<Channel*>(events[i].data.ptr);
		ioEvents.push_back(ioEvent);
	}
}
