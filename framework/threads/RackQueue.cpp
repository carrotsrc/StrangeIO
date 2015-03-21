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
	pool = ThreadPool(size);
}

void RackQueue::setSize(int size) {
	pool.setSize(size);
}

int RackQueue::getSize() {
	return pool.getSize();
}

void RackQueue::init() {
	pool.init(&mCondition, &mSharedMutex, &pump);
}

bool RackQueue::cycle() {
	std::vector< std::unique_ptr<WorkerPackage> >::iterator it;
	if(!qmutex.try_lock())
		return false;

	loadThreads(it);
	qmutex.unlock();
	return true;
}

void RackQueue::addPackage(std::function<void()> run) {

	// Pump is thread safe
	mPump.addPackage(std::unique_ptr<WorkerPackage>(new WorkerPackage(run)));
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
