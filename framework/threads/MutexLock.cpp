#include "MutexLock.h"
using namespace RackoonIO;

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
