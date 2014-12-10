#include "Reactor.h"
#include <cassert>

using namespace std;

Reactor::Reactor()
{
	epollFd = epoll_create(1024);
	assert(epollFd > 0); // TODO:
}

Reactor::~Reactor()
{
	close(epollFd); // TODO: close all fd
}

void Reactor::regist(int fd, int _ev, function<void(Reactor*,int,int)> fun)
{
	switch (ev) {
	case EPOLLIN:
		rhandleMap[fd] = fun;
		break;
	case EPOLLOUT:
		whandleMap[fd] = fun;
		break;
	case EPOLLERR:
		ehandleMap[fd] = fun;
		break;
	}

	auto it = eventMap.find(fd);
	if (it == eventMap.end()) {
		eventMap[fd] = _ev;
		epollCtl(EPOLL_CTL_ADD, fd, _ev);
	} else {
		int ev = it->second;
		ev |= _ev;
		eventMap[fd] = ev;
		epollCtl(EPOLL_CTL_MOD, fd, _ev);
	}
}

void unregist(int fd, int _ev)
{
	auto it = eventMap.find(fd);
	if (it != eventMap.end()) {
		int ev = it->second;
		ev &= ~_ev;
		eventMap[fd] = ev;
		epollCtl(EPOLL_CTL_MOD, fd, _ev);
	}
}

void Reactor::react(int maxEvent) 
{ 
	struct epoll_event evs[maxEvent];

	int n, fd, ev;
	do {
		n = epoll_wait(epollFd, evs, maxEvent, -1);
		assert(n >= 0);

		for (int i = 0; i < n; i++) {
			fd = evs[i].events;
			ev = evs[i].data.fd;
			if (ev & EPOLLIN) {
				// TODO: close handle
				rhandleMap[fd](this, fd, ev);
			}
			if (ev & EPOLLOUT) {
				whandleMap[fd](this, fd, ev);
			}
			if (ev & EPOLLERR) {
				ehandleMap[fd](this, fd, ev);
				// TODO: other error handle
			}
		}
	} while (true);
}

void Reactor::epollCtl(int cmd, int fd, int ev)
{
	struct epoll_event eev;
	eev.events = ev;
	eev.data.fd = fd;

	int err = epoll_ctl(epollFd, cmd, fd, &ev);
	assert(err == 0); // TODO 
}


