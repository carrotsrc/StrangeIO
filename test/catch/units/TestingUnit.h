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
#define CONCURRENT_TEST 104.0f

#define CYCLE_SYNC 300u
#define CYCLE_CONCURRENT 301u
#define CYCLE_EVENTLOOP 302u

class TestingUnit : public StrangeIO::RackUnit
{
public:
	TestingUnit(std::string rutype) : StrangeIO::RackUnit(rutype) { };
	void setFeedCheck(int* feeder);
	void setCycleCheck(int* cycle);
	void toggleConcurrentTest(bool flag);

protected:
	int* mFeed, *mCycle;

	bool mConcurrent;
};

} // Testing
} // StrangeIO
#endif

