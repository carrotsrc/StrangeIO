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
	unsigned int mCycleType;
	unsigned int mEventType;

	void exportedMethod(int value);

	void eventCallback(std::shared_ptr<StrangeIO::EventMessage> msg);
};
#endif
