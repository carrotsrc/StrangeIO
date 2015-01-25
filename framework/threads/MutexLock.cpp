#include "MutexLock.h"

MutexLock::MutexLock() {

}

void MutexLock::lock() {
	mlock.lock();
}

bool MutexLock::tryLock() {
	return mlock.try_lock();
}

void MutexLock::unlock() {
	mlock.unlock();
}
