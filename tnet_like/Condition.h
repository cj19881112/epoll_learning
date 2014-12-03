#ifndef CONDITION_H
#define CONDITION_H

#include "Noncopyable.h"
#include "Mutex.h"
#include <pthread.h>

class Condition : public Noncopyable {
public:
	Condition()
	{
		pthread_cond_init(&_pthreadCond, 0);
	}
	~Condition()
	{
		pthread_cond_destroy(&_pthreadCond);
	}
	void wait(Mutex &mutex)
	{
		pthread_cond_wait(&_pthreadCond, mutex.getPthreadMutex());
	}
	void notify()
	{
		pthread_cond_signal(&_pthreadCond);
	}
	void notifyAll()
	{
		pthread_cond_broadcast(&_pthreadCond);
	}
private:
	pthread_cond_t _pthreadCond;
};

#endif


