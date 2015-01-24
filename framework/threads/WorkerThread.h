#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H
#include "common.h"
namespace RackoonIO {

class WorkerThread {
	std::thread *worker;
	bool running;

protected:
	void process();

public:
	WorkerThread(bool = false);
	void start();
};

}
#endif
