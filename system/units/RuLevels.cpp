#include "RuLevels.h"

using namespace RackoonIO;
RuLevels::RuLevels()
: RackUnit() {
	addJack("audio", JACK_SEQ);
	addPlug("audio_out");
}

FeedState RuLevels::feed(Jack *jack) {
	short *period = NULL;
	Jack *out = getPlug("audio_out")->jack;
	out->frames = jack->frames;
	jack->flush(&period);
	if(out->feed(period) == FEED_WAIT)
		return FEED_WAIT;

	return FEED_OK;
}
RackState RuLevels::init() {
	workState = READY;
	cout << "RuLevels: Initialised" << endl;
	return RACK_UNIT_OK;
}

RackState RuLevels::cycle() {
	return RACK_UNIT_OK;
}

void RuLevels::setConfig(string config, string value) {

}
