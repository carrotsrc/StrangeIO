#include "RuPitchBender.h"

using namespace RackoonIO;

RuPitchBender::RuPitchBender()
: RackUnit() {
	addJack("audio", JACK_SEQ);
	addPlug("audio_out");
	workState = IDLE;
	bend = 5;
	bentPeriod = nullptr;
	oldBend = 0;
}


FeedState RuPitchBender::feed(Jack *jack) {
	Jack *out = getPlug("audio_out")->jack;
	int frames = jack->frames;

	if(bentPeriod != nullptr && out->feed(bentPeriod) == FEED_WAIT)
		return FEED_WAIT;


	return FEED_OK;
}

RackState RuPitchBender::init() {
	workState = READY;
	cout << "RuPitchBender: Initialised" << endl;
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
