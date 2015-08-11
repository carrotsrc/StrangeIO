#include "TestingUnit.h"
#include <atomic>
using namespace StrangeIO::Testing;

void TestingUnit::setFeedCheck(int* feeder) {
	mFeed = feeder;
}
