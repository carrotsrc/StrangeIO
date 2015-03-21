#include <iostream>
#include <atomic>
#include "framework/threads/RackQueue.h"
std::atomic<int> total(0);
void runnerA() {
	total++;
}

void runnerB() {
	total++;
}

void runnerC() {
	total++;
}

void cycle2000(RackoonIO::RackQueue *queue) {

	int j = 0;
	for(int i = 0; i < 1000; i++) {
		if(j == 0) 
			queue->addPackage(std::bind(&runnerA));
		else
		if(j == 1)
			queue->addPackage(std::bind(&runnerB));
		else
		if(j == 2) {
			j = -1;
			queue->addPackage(std::bind(&runnerC));
		}

		j++;
	}

	cout << "Finished" << endl;
}

int main( void ) {
	RackoonIO::RackQueue queue(3);
	queue.init();
	std::thread t(&cycle2000, &queue);
	int k;
	cin >> k;
	queue.stop();
	cout << "Total: " << total << endl;
}
