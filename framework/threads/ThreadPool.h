#ifndef THREADPOOL_H
#define THREADPOOL_H
#include "common.h"
namespace RackoonIO {

template<class T>
class ThreadPool {
	int size;
	std::vector< std::shared_ptr<T> > pool;

public:
	ThreadPool();
	ThreadPool(int);

	void setSize(int);
	int getSize();

	void init();
	std::shared_ptr<T> getThread(int index);
	std::shared_ptr<T> &operator[] (int);
};

}
#endif
