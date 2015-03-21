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
#include "WorkerThread.h"
using namespace RackoonIO;

WorkerThread::WorkerThread(std::condition_variable *condition, std::mutex *mutex, PackagePump *pump) {
	mCondition = condition;
	mSharedMutex = mutex;
	mPump = pump;
}

void WorkerThread::start() {
	worker = new std::thread(&WorkerThread::process, this);
	running = true;
	busy = false;
}

void WorkerThread::stop() {
	running = false;
}

void WorkerThread::process() {
	std::unique_lock<std::mutex> lock(*mSharedMutex);
	while(running) {
		mCondition->wait(lock);
			current = std::move(mPump->nextPackage());
			current->run();
	}
}

bool WorkerThread::isBusy() {
	return busy;
}

bool WorkerThread::assignPackage(std::unique_ptr<WorkerPackage> package) {

	if(package == nullptr) return false;
	if(busy) return false;
	if(!pkg_lock.try_lock()) return false;

	current = std::move(package);
	busy = true;
	pkg_lock.unlock();

	return true;
}



void WorkerThread::setSleep(std::chrono::microseconds us) {
}
