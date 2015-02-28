#include "RuSine.h"
using namespace RackoonIO;

RuSine::RuSine()
: RackUnit(std::string("RuSine")) {
	workState = IDLE;
}

FeedState RuSine::feed(Jack *jack) {
	return FEED_OK;
}

void RuSine::setConfig(string config, string value) {
}

RackState RuSine::init() {
	workState = READY;
	return RACK_UNIT_OK;
}

RackState RuSine::cycle() {
	
	return RACK_UNIT_OK;
}

void RuSine::block(Jack *jack) {
	
}
