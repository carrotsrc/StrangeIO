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

using namespace StrangeIO;

ThreadPool::ThreadPool() {

}

ThreadPool::ThreadPool(int nThreads) {
	size = nThreads;
}

ThreadPool::~ThreadPool() {
	stop();
	for(auto t : pool) {
		delete t;
	}
}
void ThreadPool::setSize(int nThreads) {
	size = nThreads;
}

int ThreadPool::getSize() {
	return size;
}

void ThreadPool::init(std::condition_variable *cv) {
	bool running;
	for(int i = 0; i < size; i++) {
		pool.push_back(new WorkerThread(cv));
		running = false;

		while(!running)
			running = pool[i]->isRunning();
	}
}

void ThreadPool::stop() {
	for(int i = 0; i < size; i++) {
		if(pool[i]->isActive()) {
			pool[i]->stop();
		}
	}
}

WorkerThread* ThreadPool::getThread(int index) {
	if(index >= size)
		return NULL;

	return pool[index];
}

WorkerThread* &ThreadPool::operator[] (int index) {
	return pool[index];
}

