#include "system/units/RuFlacLoad.h"
#include "system/units/RuAlsa.h"
#include "framework/threads/RackQueue.h"

class TestTasks {
	RackoonIO::RackQueue *queue;
	int checker;
public:
	TestTasks(RackoonIO::RackQueue *rq, int val) {
		queue = rq;
		checker = val;
	};

	void runFirst() {
		cout << "1st: Value is - " << checker << endl;
	};

	void runSecond() {
		cout << "2nd: Value is - " << checker << endl;
	};

	void runThird() {
		cout << "3rd: Value is - " << checker << endl;
	};

	void addTask() {
		queue->addPackage(std::bind(&TestTasks::runFirst, this));
		queue->addPackage(std::bind(&TestTasks::runSecond, this));
		queue->addPackage(std::bind(&TestTasks::runThird, this));
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
	TestTasks tester(&pool, 808);

	pool.init();
	tester.addTask();
	pool.cycle();
	int x;
	cin >> x;
}




