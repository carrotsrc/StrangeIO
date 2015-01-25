#include "RackQueue.h"
using namespace RackoonIO;

RackQueue::RackQueue(int size) {
	pool = new ThreadPool<WorkerThread>(size);
}

void RackQueue::setSize(int size) {
	pool->setSize(size);
}

int RackQueue::getSize() {
	return pool->getSize();
}

void RackQueue::init() {
	pool->init();
}

void RackQueue::start() {
	running = true;
	int sz = 0;
	std::vector< std::unique_ptr<WorkerPackage> >::iterator it;
	while(running) {
		if((sz = queue.size()) && tryLock()) {
			loadThreads(it);
			unlock();
		}

		std::this_thread::sleep_for(std::chrono::microseconds(200));
	}
}

void RackQueue::cycle() {
	if(!running)
		return;

	std::vector< std::unique_ptr<WorkerPackage> >::iterator it;
	loadThreads(it);
}

inline void RackQueue::loadThreads(std::vector< std::unique_ptr<WorkerPackage> >::iterator it) {
	int tindex, sz = pool->getSize();

	for(it = queue.begin(); it != queue.end(); it++ ) {
		for(tindex = 0; tindex < sz; tindex++)
			if((*pool)[tindex]->assignPackage(std::move(*it)))
				it = queue.erase(it);
	}
}

void RackQueue::addPackage(std::function<void()> run) {
	lock();
	queue.push_back(std::unique_ptr<WorkerPackage>(new WorkerPackage(run)));
	unlock();
}

bool RackQueue::tryAddPackage(std::function<void()> run) {
	if(!tryLock())
		return false;

	queue.push_back(std::unique_ptr<WorkerPackage>(new WorkerPackage(run)));
	unlock();
	return true;
}
