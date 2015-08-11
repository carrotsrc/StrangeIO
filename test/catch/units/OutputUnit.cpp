#include "OutputUnit.h"

using namespace StrangeIO;

OutputUnit::OutputUnit()
: TestingUnit(std::string("OutputUnit")) {
	workState = IDLE;
	addJack("audio", JACK_SEQ, 1);
	mFeed = nullptr;
}

FeedState OutputUnit::feed(Jack *jack) {
	if(mFeed == nullptr)
		return FEED_WAIT;

	(*mFeed)++;
	PcmSample* period;
	jack->flush(&period,1);

	if(*period == FEED_TEST) {
		return FEED_OK;
	}

	return FEED_WAIT;

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
