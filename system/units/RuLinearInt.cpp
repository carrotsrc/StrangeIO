#include "RuLinearInt.h"

using namespace RackoonIO;

RuLinearInt::RuLinearInt()
: RackUnit() {
	addJack("audio", JACK_SEQ);
	addPlug("audio_out");
	workState = IDLE;
}


FeedState RuLinearInt::feed(Jack *jack) {
	Jack *out = getPlug("audio_out")->jack;
	int frames = jack->frames;
	short *period;
	jack->flush(&period);
	out->frames = jack->frames;
	return out->feed(period);
}

RackState RuLinearInt::init() {
	workState = READY;
	cout << "RuLinearInt: Initialised" << endl;
	return RACK_UNIT_OK;
}

RackState RuLinearInt::cycle() {
	return RACK_UNIT_OK;
}

void RuLinearInt::setConfig(string config, string value) {

}

void RuLinearInt::block(Jack *jack) {
	Jack *out = getPlug("audio_out")->jack;
	out->block();
}
