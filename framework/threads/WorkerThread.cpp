#include "WorkerThread.h"
#include <chrono>
using namespace RackoonIO;

WorkerThread::WorkerThread(bool autoStart) {
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

		std::this_thread::sleep_for(std::chrono::microseconds(120));
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
