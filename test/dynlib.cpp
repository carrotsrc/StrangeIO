#include "dynlib.h"
#include "framework/common.h"

TestAdaptor::TestAdaptor() : RackUnit("TestAdaptor") {
	addJack("power", JACK_SEQ);
}

RackState TestAdaptor::init() {
	UnitMsg("Initialised");
	return RACK_UNIT_OK;
}

void TestAdaptor::setConfig(std::string, std::string) {

}

FeedState TestAdaptor::feed(Jack *jack) {
	UnitMsg("Feeding");
	return FEED_OK;
}

RackState TestAdaptor::cycle() {
	UnitMsg("Cycle");
	return RACK_UNIT_OK;
}

void TestAdaptor::block(Jack*) {
}

DynamicBuilder(TestAdaptor);
