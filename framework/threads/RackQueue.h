#ifndef RACKQUEUE_H
#define RACKQUEUE_H
#include "WorkerThread.h"
#include "ThreadPool.h"

namespace RackoonIO {

class RackQueue : public MutexLock {
	ThreadPool<WorkerThread> *pool;
	std::vector< std::unique_ptr<WorkerPackage> > queue;
	bool running;

	inline void loadThreads(std::vector< std::unique_ptr<WorkerPackage> >::iterator it);
public:
	RackQueue(int);

	void setSize(int);
	int getSize();

	void setSleep(std::chrono::microseconds);
	void init();
	void start();
	bool cycle();

	int getQueueSize() {
		return queue.size();
	}

	void addPackage(std::function<void()> run);
	bool tryAddPackage(std::function<void()> run);
};

}
#endif
