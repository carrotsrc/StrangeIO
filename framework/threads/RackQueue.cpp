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
	pool.init(&mCondition, &mSharedMutex, &mPump);
}

void RackQueue::addPackage(std::function<void()> run) {
	// Pump is thread safe
	std::unique_lock<std::mutex> lock(mSharedMutex);
	mPump.addPackage(std::unique_ptr<WorkerPackage>(new WorkerPackage(run)));
	lock.unlock();
	mCondition.notify_one();
}
