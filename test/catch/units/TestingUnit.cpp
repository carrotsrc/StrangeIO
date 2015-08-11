#include "TestingUnit.h"
#include <atomic>
using namespace StrangeIO::Testing;

void TestingUnit::setFeedCheck(int* feeder) {
	mFeed = feeder;
}

void TestingUnit::setCycleCheck(int* cycle) {
	mCycle = cycle;
}
