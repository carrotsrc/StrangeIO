#include "RuPitchBender.h"

using namespace RackoonIO;

RuPitchBender::RuPitchBender()
: RackUnit() {
	addJack("audio", JACK_SEQ);
	addPlug("audio_out");
	workState = IDLE;
	framesIn = framesOut = nullptr;
	sampleRate = 44100;
	convRate = 46300;
	ratio = (double)convRate/(double)sampleRate;
	convPeriod = nullptr;
}

RuPitchBender::~RuPitchBender() {

	if(framesIn == nullptr) {
		free(framesIn);
		free(framesOut);
	}
}


FeedState RuPitchBender::feed(Jack *jack) {
	Jack *out = getPlug("audio_out")->jack;
	int frames = jack->frames, err = 0;

	if(convPeriod != nullptr && out->feed(convPeriod) == FEED_WAIT)
			return FEED_WAIT;
	short *period;
	
	if(framesOut == nullptr) {
		framesOut = (float*)malloc(sizeof(float)*(frames<<2));
		framesIn = (float*)malloc(sizeof(float)*(frames));
	}

	jack->flush(&period);
	out->frames = frames;
	return out->feed(period);

	if(out->feed(period) == FEED_WAIT)
		return FEED_OK;

	convPeriod = nullptr;
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
