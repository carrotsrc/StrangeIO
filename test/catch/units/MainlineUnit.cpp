#include "MainlineUnit.h"

using namespace StrangeIO;
using namespace StrangeIO::Testing;

MainlineUnit::MainlineUnit()
: TestingUnit(std::string("MainlineUnit")) {
	workState = IDLE;
	addPlug("audio_out");
	addJack("power", JACK_AC, 1);
	MidiExport("exported", MainlineUnit::exportedMethod);
	mAtomFeed = nullptr;
}

FeedState MainlineUnit::feed(Jack *jack) {
	if(mFeed == nullptr)
		return FEED_WAIT;

	(*mFeed)++;
	return FEED_OK;
}
void MainlineUnit::setConfig(std::string config, std::string value) {
}
	RackState MainlineUnit::init() {
	workState = READY;
	return RACK_UNIT_OK;
}

RackState MainlineUnit::cycle() {
	return RACK_UNIT_OK;
}

void MainlineUnit::block(Jack *jack) {
}

void MainlineUnit::exportedMethod(int value) {

}

DynamicBuilder(MainlineUnit);
