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
#include "PackagePump.h"

using namespace RackoonIO;

PackagePump::PackagePump() {
}
PackagePump::~PackagePump() {
	for(auto it = mQueue.begin(); it != mQueue.end();) {
		(*it).reset();
		it = mQueue.erase(it);
	}
}

void PackagePump::addPackage(std::unique_ptr<WorkerPackage> pkg) {
	mQueueMutex.lock();
		mQueue.push_back(std::move(pkg));
	mQueueMutex.unlock();
}

std::unique_ptr<WorkerPackage> PackagePump::nextPackage() {
	std::unique_ptr<WorkerPackage> pkg = nullptr;
	mQueueMutex.lock();
		if(mQueue.size() > 0) {
			auto it = mQueue.begin();
			pkg = std::unique_ptr<WorkerPackage>(std::move(*it));
			mQueue.erase(it);
		}
	mQueueMutex.unlock();

	return std::move(pkg);
}

int PackagePump::getLoad() {
	return mQueue.size();
}
