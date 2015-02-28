#include "RuFresh.h"
using namespace RackoonIO;

RuFresh::RuFresh()
: RackUnit(std::string("RuFresh")) {
	addJack("audio", JACK_SEQ);
	workState = IDLE;
}

FeedState RuFresh::feed(Jack *jack) {
	Jack *j = getJack("audio");
	return FEED_OK;
}

void RuFresh::setConfig(string config, string value) {
}

RackState RuFresh::init() {
	workState = READY;
	return RACK_UNIT_OK;
}

RackState RuFresh::cycle() {
	
	return RACK_UNIT_OK;
}

void RuFresh::block(Jack *jack) {
	
}
