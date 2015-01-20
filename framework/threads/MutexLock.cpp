#include "MutexLock.h"
MutexLock::MutexLock(void) {
	pthread_mutex_init(&mutex, NULL);
}

void MutexLock::reassignMutex(pthread_mutex_t type, const pthread_mutexattr_t *attr) {
	pthread_mutex_init(&mutex, attr);
}
pthread_mutex_t *MutexLock::getMutex() {
	return &mutex;
}

int MutexLock::lock() {
	return pthread_mutex_lock(&mutex);
}

int MutexLock::tryLock() {
	return pthread_mutex_trylock(&mutex);
}

int MutexLock::unlock() {
	return pthread_mutex_unlock(&mutex);
}
