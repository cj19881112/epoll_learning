#include "Reactor.h"
#include <cassert>
#include <sys/epoll.h>
#include <unistd.h>

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

void Reactor::regist(int fd, int _ev, function<void(Reactor*,int,int)> fn)
{
	switch (_ev) {
	case EPOLLIN:
		rhandleMap[fd] = fn;
		break;
	case EPOLLOUT:
		whandleMap[fd] = fn;
		break;
	case EPOLLERR:
		ehandleMap[fd] = fn;
		break;
	default:
		assert(0);
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

void Reactor::unregist(int fd, int _ev)
{
	auto it = eventMap.find(fd);
	if (it != eventMap.end()) {
		int ev = it->second;
		ev &= ~_ev;
		eventMap[fd] = ev;
		epollCtl(EPOLL_CTL_MOD, fd, _ev);
	}
}

void Reactor::remove(int fd)
{
	auto it = eventMap.find(fd);
	if (it != eventMap.end()) {
		eventMap.erase(it);
		epollCtl(EPOLL_CTL_DEL, fd, EPOLLERR);
	}
}

void Reactor::react(int timeOut, int maxEvent) 
{ 
	struct epoll_event evs[maxEvent];

	int n, fd, ev;
	do {
		n = epoll_wait(epollFd, evs, maxEvent, timeOut);
		assert(n >= 0);

		for (int i = 0; i < n; i++) {
			fd = evs[i].data.fd;
			ev = evs[i].events;
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

	int err = epoll_ctl(epollFd, cmd, fd, &eev);
	assert(err == 0); // TODO 
}


