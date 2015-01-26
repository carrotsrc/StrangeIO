#ifndef THREADPOOL_H
#define THREADPOOL_H
#include "common.h"
namespace RackoonIO {

template<class T>
class ThreadPool {
	int size;
	std::vector< T* > pool;

public:
	ThreadPool();
	ThreadPool(int);

	void setSize(int);
	int getSize();

	void init();
	T* getThread(int index);
	T* &operator[] (int);
};

}
#endif
