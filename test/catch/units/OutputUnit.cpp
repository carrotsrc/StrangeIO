#include "OutputUnit.h"

using namespace StrangeIO;

OutputUnit::OutputUnit()
: TestingUnit(std::string("OutputUnit")) {
	workState = IDLE;
	addJack("audio", JACK_SEQ, 1);
}

FeedState OutputUnit::feed(Jack *jack) {
	return FEED_OK;
}
void OutputUnit::setConfig(std::string config, std::string value) {
}
	RackState OutputUnit::init() {
	workState = READY;
	return RACK_UNIT_OK;
}

RackState OutputUnit::cycle() {
	return RACK_UNIT_OK;
}

void OutputUnit::block(Jack *jack) {
}

DynamicBuilder(OutputUnit);
