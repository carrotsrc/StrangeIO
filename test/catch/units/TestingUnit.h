#ifndef __TESTINGUNIT_H_1439314635__
#define __TESTINGUNIT_H_1439314635__
#include "framework/rack/RackUnit.h"
#include<atomic>
namespace StrangeIO {
namespace Testing {


class TestingUnit : public StrangeIO::RackUnit
{
public:
	TestingUnit(std::string rutype) : StrangeIO::RackUnit(rutype) { };
	void setFeedCheck(int* feeder);

protected:
	int* mFeed;
};

} // Testing
} // StrangeIO
#endif

