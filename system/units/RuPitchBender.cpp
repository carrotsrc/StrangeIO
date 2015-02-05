#include "RuPitchBender.h"

using namespace RackoonIO;

RuPitchBender::RuPitchBender()
: RackUnit() {
	addJack("audio", JACK_SEQ);
	addPlug("audio_out");
	workState = IDLE;
	framesIn = framesOut = nullptr;
	sampleRate = 44100;
	convRate = 44100;
	ratio = (double)convRate/(double)sampleRate;
	convPeriod = nullptr;
	resampler = nullptr;
	MIDI_BIND("pitchBend", RuPitchBender::midiBend);
}

RuPitchBender::~RuPitchBender() {

	if(framesIn != nullptr) {
		free(framesIn);
		free(framesOut);
	}
}

void RuPitchBender::actionResample() {
	int usedFrames = 0;
	nResampled = resample_process(resampler, ratio, framesIn,
			nFrames, 0, &usedFrames, framesOut, nFrames<<1);
	convPeriod = (short*)malloc(sizeof(short)*nResampled);
	
	for(int i = 0; i < nResampled; i++)
		convPeriod[i] = framesOut[i];

	workState = FLUSHING;
}


FeedState RuPitchBender::feed(Jack *jack) {
	if(workState != READY)
		return FEED_WAIT;

	nFrames = jack->frames;
	short *period;

	jack->flush(&period);
	if(ratio == 1) {
		Jack *out = getPlug("audio_out")->jack;
		out->frames = jack->frames;
		return out->feed(period);
	}

	
	if(framesOut == nullptr) {
		framesOut = (float*)malloc(sizeof(float)*(nFrames<<1));
		framesIn = (float*)malloc(sizeof(float)*(nFrames));
	}

	for(int i = 0; i < nFrames; i++)
		framesIn[i] = period[i];
	free(period);
	
	OUTSRC(RuPitchBender::actionResample);
	workState = RESAMPLING;

	return FEED_OK;
}

RackState RuPitchBender::init() {
	resampler = resample_open(1, 0.92, 1.08);
	int fwidth = resample_get_filter_width(resampler);
	cout << "RuPitchBender: Initialised" << endl;
	workState = READY;
	return RACK_UNIT_OK;
}

RackState RuPitchBender::cycle() {
	if(workState == FLUSHING) {
		Jack *out = getPlug("audio_out")->jack;
		out->frames = nResampled;
		if(out->feed(convPeriod) == FEED_OK)
			workState = READY;
	}

	return RACK_UNIT_OK;
}

void RuPitchBender::setConfig(string config, string value) {

}

void RuPitchBender::block(Jack *jack) {
	Jack *out = getPlug("audio_out")->jack;
	out->block();
}

void RuPitchBender::midiBend(int value) {
	if(value == 64) {
		ratio = 1;
	}

	if(value < 64) {
		ratio = 1-((64-(double)value)*.00125);
	} else {
		ratio = 1+((64-(127-(double)value))*.00125);
	}
}
