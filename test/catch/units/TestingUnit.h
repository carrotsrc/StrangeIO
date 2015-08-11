#ifndef __TESTINGUNIT_H_1439314635__
#define __TESTINGUNIT_H_1439314635__
#include "framework/rack/RackUnit.h"
#include<atomic>
namespace StrangeIO {
namespace Testing {

#define CHANNEL_TWO 808.0f
#define SINGLE_CHANNEL 100.0f
#define DOUBLE_CHANNEL 101.0f
#define FEED_TEST 102.0f
#define CYCLE_TEST 103.0f

class TestingUnit : public StrangeIO::RackUnit
{
public:
	TestingUnit(std::string rutype) : StrangeIO::RackUnit(rutype) { };
	void setFeedCheck(int* feeder);
	void setCycleCheck(int* cycle);

protected:
	int* mFeed, *mCycle;
};

} // Testing
} // StrangeIO
#endif

