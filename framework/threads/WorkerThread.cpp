#include "WorkerThread.h"
using namespace RackoonIO;

WorkerThread::WorkerThread(bool autoStart) {
	uSleep = std::chrono::microseconds(120);
	if(autoStart)
		start();
}

void WorkerThread::start() {
	worker = new std::thread(&WorkerThread::process, this);
	running = true;
}

void WorkerThread::stop() {
	running = false;
}

void WorkerThread::process() {
	while(running) {
		if(busy) {
			lock();
			current->run();
			busy = false;
			unlock();
		}
		std::this_thread::sleep_for(uSleep);
	}
}

bool WorkerThread::isBusy() {
	return busy;
}

bool WorkerThread::assignPackage(std::unique_ptr<WorkerPackage> package) {
	if(package == nullptr) return false;
	if(busy) return false;

	if(!tryLock()) return false;
	current = std::move(package);
	busy = true;
	unlock();

	return true;
}


void WorkerThread::setSleep(std::chrono::microseconds us) {
	std::cout << "Set sleep to " << us.count() << "us for " << worker->get_id() << std::endl;
	uSleep = us;
}
