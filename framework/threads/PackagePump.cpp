#include "PackagePump.h"

using namespace RackoonIO;

PackagePump::PackagePump() {
}

void PackagePump::addPackage(std::unique_ptr<WorkerPackage> pkg) {
	mQueueMutex.lock();
		mQueue.push_back(std::move(pkg));
	mQueueMutex.unlock();
}

std::unique_ptr<WorkerPackage> PackagePump::nextPackage() {
	mQueueMutex.lock();
		auto it = mQueue.begin();
		auto pkg = std::unique_ptr<WorkerPackage>(std::move(*it));
		mQueue.erase(it);
	mQueueMutex.unlock();

	return pkg;
}

int PackagePump::getLoad() {
	return mQueue.size();
}
