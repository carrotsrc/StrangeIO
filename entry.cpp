#include "system/units/RuFlacLoad.h"
#include "system/units/RuAlsa.h"
#include "framework/threads/RackQueue.h"

class TestTasks {
	RackoonIO::RackQueue *queue;

public:
	TestTasks(RackoonIO::RackQueue *rq) {
		queue = rq;
	};

	void runFirst() {
		cout << "Running task FIRST" << endl;
	};

	void addTask() {
		queue->addPackage(std::bind(&TestTasks::runFirst, this));
	};
};

int main(void)
{
/*
	RuFlacLoad f;
	RuAlsa a;
	f.setConnection("audio_out", "audio", &a);
	f.setConfig("filename", "/home/charlie/Brain in a Fish Tank.flac");
	f.getJack("power")->rackFeed(RACK_AC);
*/
	RackoonIO::RackQueue pool(6);
	TestTasks tester(&pool);

	pool.init();
	tester.addTask();
	pool.cycle();
	int x;
	cin >> x;
}




