#ifndef SAFELOCK_H
#define SAFELOCK_H
#include "common.h"
class MutexLock
{
protected:
	std::mutex mlock;
public:
	MutexLock(void);

	void lock();
	bool tryLock();
	void unlock();
};
#endif
