#ifndef MUTEX_H
#define MUTEX_H

#include "Noncopyable.h"
#include <pthread.h>

class Mutex : public Noncopyable {
public:
	Mutex()
	{
		pthread_mutex_init(&_pthreadMutex, 0);
	}
	~Mutex()
	{
		pthread_mutex_destroy(&_pthreadMutex);
	}

	void lock()
	{
		pthread_mutex_lock(&_pthreadMutex);
	}

	void unlock()
	{
		pthread_mutex_unlock(&_pthreadMutex);
	}

	pthread_mutex_t *getPthreadMutex()
	{
		return &_pthreadMutex;
	}
private:
	pthread_mutex_t _pthreadMutex;
};

class MutexGuard : public Noncopyable {
public:
	MutexGuard(Mutex &mutex)
		: _mutex(mutex)
	{
		_mutex.lock();
	}
	~MutexGuard()
	{
		_mutex.unlock();
	}
private:
	Mutex &_mutex;
};

#endif
