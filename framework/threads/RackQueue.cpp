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
	mPoolSize = size;
}

void RackQueue::setSize(int size) {
	pool.setSize(size);
	mPoolSize = size;
}

int RackQueue::getSize() {
	return pool.getSize();
}

void RackQueue::init() {
	pool.init(&mCycleCondition);
	mWaiter = std::thread(&RackQueue::cycle, this);
	mRunning = true;
}

void RackQueue::addPackage(std::function<void()> run) {
	// Pump is thread safe
	mPump.addPackage(std::unique_ptr<WorkerPackage>(new WorkerPackage(run)));
	mCycleCondition.notify_one();
}

void RackQueue::stop() {
	pool.stop();
	mRunning = false;
	mCycleCondition.notify_one();
	mWaiter.join();
}

int RackQueue::getPumpLoad() {
	return mPump.getLoad();
}

/*
 * There are two conditions for this trigger a
 * dispatch attempt of another job - a new job 
 * has been added or a thread has finished a job.
 *
 * Dispatch should be done centrally within this
 * method to avoid race conditions
 */
void RackQueue::cycle() {
	std::unique_lock<std::mutex> lock(mMutex);
	WorkerPackage *rawPkg = nullptr;
	while(mRunning) {
		std::unique_ptr<WorkerPackage> pkg = nullptr;
		int i = 0;
		double j = 0;
		bool assigned = false;

		mCycleCondition.wait(lock);

		if(!mRunning) {
			lock.unlock();
			break;
		}

		if(rawPkg == nullptr) {
			pkg = mPump.nextPackage();
			rawPkg = pkg.release();
		}

		if(rawPkg == nullptr) {
			std::cout << "Exhausted pump" << std::endl;
			continue;
		}

		if(assign(rawPkg))
			rawPkg = nullptr;
	}
	
}

bool RackQueue::assign(WorkerPackage *pkg) {
	for(int i = 0; i < mPoolSize; i++) {
		if(pool[i]->isWaiting()) {
			pool[i]->assignPackage(std::unique_ptr<WorkerPackage>(pkg));
			pool[i]->notify();
			return true;
		}
	}
	return false;
}
