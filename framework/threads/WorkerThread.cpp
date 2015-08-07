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
using namespace StrangeIO;

WorkerThread::WorkerThread(std::condition_variable *cv) {
	mReadyCondition = cv;
	start();
}

void WorkerThread::start() {
	mRunning = false;
	mWorker = std::thread(&WorkerThread::process, this);
}

void WorkerThread::stop() {
	mRunning = false;
	mCondition.notify_one();
	mWorker.join();
}

void WorkerThread::process() {
	std::unique_lock<std::mutex> lock(mMutex);
	mLoaded = false;
	mRunning = true;
	while(mRunning) {
		mCondition.wait(lock);
			if(!mRunning) {
				current.reset();
				lock.unlock();
				break;
			}
			current->run();
			mLoaded = false;
			mReadyCondition->notify_all();
	}
}

bool WorkerThread::isRunning() {
	return mRunning;
}

bool WorkerThread::assignPackage(std::unique_ptr<WorkerPackage> pkg, bool unlock) {
	current = std::move(pkg);
	mLoaded = true;
	if(unlock)
		mMutex.unlock();
	return true;
}

bool WorkerThread::isLoaded() {
	return mLoaded;
}

void WorkerThread::notify() {
	mCondition.notify_all();
}

bool WorkerThread::isWaiting() {
	if(!mMutex.try_lock()) {
		return false;
	}

	if(mLoaded) {
		mMutex.unlock();
		return false;
	}
	return true;
}
