#include "ThreadPool.h"
using namespace RackoonIO;

ThreadPool::ThreadPool() {

}

ThreadPool::ThreadPool(int nThreads) {
	size = nThreads;
}

void ThreadPool::setSize(int nThreads) {
	size = nThreads;
}

int ThreadPool::getSize() {
	return size;
}

void ThreadPool::init() {
	for(int i = 0; i < size; i++)
		pool.push_back(new WorkerThread(true));
}
