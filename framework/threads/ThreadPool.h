#ifndef THREADPOOL_H
#define THREADPOOL_H
#include "common.h"
#include "WorkerThread.h"
namespace RackoonIO {

class ThreadPool {
	int size;
	std::vector< RackoonIO::WorkerThread* > pool;

public:
	ThreadPool();
	ThreadPool(int);

	void setSize(int);
	int getSize();

	void init();
};

}
#endif
