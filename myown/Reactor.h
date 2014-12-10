#ifndef REACTOR_H
#define REACTOR_H

#include <functional>
#include <map>

class Reactor {
public:
	Reactor();
	~Reactor();
	void regist(int fd, int ev, std::function<void(Reactor*,int,int)> fun);
	void unregist(int fd, int ev);
	void remove(int fd);
	void react(int timeOut = -1, int maxEvent = 128);
private:
	void epollCtl(int cmd, int fd, int ev);

	std::map<int, std::function<void(Reactor*,int,int)>> rhandleMap;
	std::map<int, std::function<void(Reactor*,int,int)>> whandleMap;
	std::map<int, std::function<void(Reactor*,int,int)>> ehandleMap;

	std::map<int, int> eventMap;

	int epollFd;
};

#endif

