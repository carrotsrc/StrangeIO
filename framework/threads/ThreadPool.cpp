/* Copyright 2015 Charlie Fyvie-Gauld
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published 
 *  by the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
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
		pool.push_back(new T(true));
}

template <class T>
T* ThreadPool<T>::getThread(int index) {
	if(index >= size)
		return NULL;

	return pool[index];
}

template <class T>
T* &ThreadPool<T>::operator[] (int index) {
	return pool[index];
}

template class ThreadPool<WorkerThread>;
