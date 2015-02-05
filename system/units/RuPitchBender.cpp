#include "RuPitchBender.h"

using namespace RackoonIO;

RuPitchBender::RuPitchBender()
: RackUnit() {
	addJack("audio", JACK_SEQ);
	addPlug("audio_out");
	workState = IDLE;
	framesIn = framesOut = nullptr;
	sampleRate = 44100;
	convRate = 40000;
	ratio = (double)convRate/(double)sampleRate;
	convPeriod = nullptr;
	resampler = nullptr;
}

RuPitchBender::~RuPitchBender() {

	if(framesIn != nullptr) {
		free(framesIn);
		free(framesOut);
	}
}


FeedState RuPitchBender::feed(Jack *jack) {
	Jack *out = getPlug("audio_out")->jack;
	int frames = jack->frames, resampledFrames = 0, usedFrames = 0;

	if(convPeriod != nullptr && out->feed(convPeriod) == FEED_WAIT)
			return FEED_WAIT;
	short *period;
	
	if(framesOut == nullptr) {
		framesOut = (float*)malloc(sizeof(float)*(frames<<5));
		framesIn = (float*)malloc(sizeof(float)*(frames));
	}

	jack->flush(&period);
	for(int i = 0; i < frames; i++)
		framesIn[i] = period[i];

	resampledFrames = resample_process(resampler, ratio, framesIn,
			frames, 0, &usedFrames, framesOut, frames<<5);
	convPeriod = (short*)malloc(sizeof(short)*resampledFrames);
	
	for(int i = 0; i < resampledFrames; i++)
		convPeriod[i] = framesOut[i];

	out->frames = resampledFrames;
	if(out->feed(convPeriod) == FEED_WAIT)
		return FEED_OK;

	convPeriod = nullptr;
	return FEED_OK;
}

RackState RuPitchBender::init() {
	workState = READY;
	resampler = resample_open(1, 0.92, 1.08);
	int fwidth = resample_get_filter_width(resampler);
	cout << "RuPitchBender: Filter width - " << fwidth << endl;
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
