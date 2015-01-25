#include "ThreadPool.h"
#include "WorkerThread.h"

using namespace RackoonIO;

template <class T>
ThreadPool<T>::ThreadPool() {

}

template <class T>
ThreadPool<T>::ThreadPool(int nThreads) {
	size = nThreads;
}


template <class T>
void ThreadPool<T>::setSize(int nThreads) {
	size = nThreads;
}

template <class T>
int ThreadPool<T>::getSize() {
	return size;
}

template <class T>
void ThreadPool<T>::init() {
	for(int i = 0; i < size; i++)
		pool.push_back(shared_ptr<T>(new T(true)));
}

template <class T>
std::shared_ptr<T> ThreadPool<T>::getThread(int index) {
	if(index >= size)
		return NULL;

	return pool[index];
}

template <class T>
std::shared_ptr<T> &ThreadPool<T>::operator[] (int index) {
	return pool[index];
}

template class ThreadPool<WorkerThread>;
