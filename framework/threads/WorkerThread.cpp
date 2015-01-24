#include "WorkerThread.h"
using namespace RackoonIO;

WorkerThread::WorkerThread(bool autoStart) {
	if(autoStart)
		start();
}

void WorkerThread::start() {
	worker = new std::thread(&WorkerThread::process, this);
}

void WorkerThread::process() {
	cout << "Worker Thread " << worker->get_id() << " running" << endl;
}
