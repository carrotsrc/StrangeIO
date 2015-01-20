#ifndef SAFELOCK_H
#define SAFELOCK_H
#include "common.h"
class MutexLock
{

public:
	pthread_mutex_t mutex;
	MutexLock(void);

	void reassignMutex(pthread_mutex_t, const pthread_mutexattr_t *restrict);
	pthread_mutex_t *getMutex();
	int lock();
	int tryLock();
	int unlock();
};
#endif
