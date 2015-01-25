#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H
#include "common.h"
#include "WorkerPackage.h"
#include "MutexLock.h"
namespace RackoonIO {

class WorkerThread : public MutexLock {
	bool busy;
	bool running;

	std::thread *worker;
	unique_ptr<WorkerPackage> current;
	
	void process();

	std::mutex pkg_lock;
public:
	WorkerThread(bool = false);
	void start();
	void stop();
	bool isBusy();

	bool assignPackage(unique_ptr<WorkerPackage>);
};

}
#endif
