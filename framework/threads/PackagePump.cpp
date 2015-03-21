#include "PackagePump.h"

using namespace RackoonIO;

PackagePump::PackagePump(std::mutex *mutex, std::condition_variable *cv) {
	mSharedMutex = mutex;
	mConditional = cv;
}

void PackagePump::addPackage(std::unique_ptr<WorkerPackage> pkg) {
	mQueueMutex.lock();
		mQueue.push_back(std::move(pkg));
	mQueueMutex.unlock();
}

std::unique_ptr<WorkerPackage> PackagePump::getPackage() {
	std::unique_ptr<WorkerPackage> pkg;
	mQueueMutex.lock();
		std::unique_lock<std::mutex> lock(mSharedMutex);
		auto it = mQueue.begin();
		pkg = std::unique_ptr<WorkerPackage>(std::move(*it));
		mQueue.push_back(std::move(pkg));
	mQueueMutex.unlock();
}
