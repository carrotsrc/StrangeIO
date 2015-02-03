#include "RuPitchBender.h"

using namespace RackoonIO;

RuPitchBender::RuPitchBender()
: RackUnit() {
	addJack("audio", JACK_SEQ);
	addPlug("audio_out");
	workState = IDLE;
}


FeedState RuPitchBender::feed(Jack *jack) {
	Jack *out = getPlug("audio_out")->jack;
	short *period;
	jack->flush(&period);
	out->feed(period);
}

RackState RuPitchBender::init() {
	workState = READY;
	return RACK_UNIT_OK;
}

RackState RuPitchBender::cycle() {
	return RACK_UNIT_OK;
}

void RuPitchBender::setConfig(string config, string value) {

}

void RuPitchBender::block(Jack *jack) {
	Jack *out = getPlug("audio_out")->jack;
	out->block();
}
