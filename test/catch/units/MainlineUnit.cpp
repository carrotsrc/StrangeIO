#include "MainlineUnit.h"

using namespace StrangeIO;

MainlineUnit::MainlineUnit()
: RackUnit(std::string("MainlineUnit")) {
	workState = IDLE;
	addPlug("audio_out");
	addJack("power", JACK_AC, 1);
	MidiExport("exported", MainlineUnit::exportedMethod);
}

FeedState MainlineUnit::feed(Jack *jack) {
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
