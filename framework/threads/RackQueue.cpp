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
		if((sz = queue.size()) && qmutex.try_lock()) {
			loadThreads(it);
			qmutex.unlock();
		}

		std::this_thread::sleep_for(std::chrono::microseconds(200));
	}
}

bool RackQueue::cycle() {
	std::vector< std::unique_ptr<WorkerPackage> >::iterator it;
	if(!qmutex.try_lock())
		return false;

	loadThreads(it);
	qmutex.unlock();
	return true;
}

inline void RackQueue::loadThreads(std::vector< std::unique_ptr<WorkerPackage> >::iterator it) {
	int tindex, sz = pool->getSize();
	bool inc = true;
	it = queue.begin();
	while(it != queue.end()) {
		inc = true;
		for(tindex = 0; tindex < sz; tindex++) {
			if(!((*pool)[tindex]->isBusy())) {
				(*pool)[tindex]->assignPackage(std::move(*it));
				it = queue.erase(it);
				inc = false;
				break;
			}
		}
		if(inc) it++;
	}
}

void RackQueue::addPackage(std::function<void()> run) {
	qmutex.lock();
	queue.push_back(std::unique_ptr<WorkerPackage>(new WorkerPackage(run)));
	qmutex.unlock();
}

bool RackQueue::tryAddPackage(std::function<void()> run) {
	if(!qmutex.try_lock())
		return false;

	queue.push_back(std::unique_ptr<WorkerPackage>(new WorkerPackage(run)));
	qmutex.unlock();
	return true;
}


void RackQueue::setSleep(std::chrono::microseconds us) {
	int sz = pool->getSize();
	for(int i = 0; i < sz; i++)
		(*pool)[i]->setSleep(us);
}
