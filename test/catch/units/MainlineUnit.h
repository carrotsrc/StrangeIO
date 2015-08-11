#ifndef __MAINLINEUNIT_H_1439299831__
#define __MAINLINEUNIT_H_1439299831__
#include <atomic>
#include "framework/rack/RackUnit.h"
#include "TestingUnit.h"
class MainlineUnit : public StrangeIO::Testing::TestingUnit
{
public:
	enum WorkState {
		IDLE,
		INIT,
		READY
	};

	MainlineUnit();
	StrangeIO::FeedState feed(StrangeIO::Jack*);
	void setConfig(std::string, std::string);
	StrangeIO::RackState init();
	StrangeIO::RackState cycle();
	void block(StrangeIO::Jack*);

	void setAtomicFeed(std::atomic<int>* atom);

private:
	WorkState workState;
	std::atomic<int>* mAtomFeed;

	void exportedMethod(int value);
};
#endif
