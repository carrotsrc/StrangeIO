#include "RuPitchBender.h"

using namespace RackoonIO;

RuPitchBender::RuPitchBender() : RackUnit() { addJack("audio", JACK_SEQ);
	addPlug("audio_out"); 
	workState = IDLE; 
	framesIn = framesOut = nullptr;
	ratio = 1.01; 
	convPeriod = nullptr;
	resampler = nullptr; 
	releasePeriod = nullptr;
	MIDI_BIND("pitchBend", RuPitchBender::midiBend); }

RuPitchBender::~RuPitchBender() {

	if(framesIn != nullptr) { 
		free(framesIn); 
		free(framesOut); 
	} 
}

void RuPitchBender::actionResample() {
	bufLock.lock();
	nResampled = resample_process(resampler, ratio, framesIn, nFrames, 0, &usedFrames,
			framesOut, nFrames<<1);

	bufLock.unlock();

}


FeedState RuPitchBender::feed(Jack *jack) {
	if(!bufLock.try_lock())
		return FEED_WAIT;

	if(workState != READY && workState != WAITING) {
		bufLock.unlock();
		return FEED_WAIT;
	}

	nFrames = jack->frames;
	short *period;

	jack->flush(&period);
	if(ratio == 1) {
		Jack *out = getPlug("audio_out")->jack;
		out->frames = jack->frames;
		bufLock.unlock();
		return out->feed(period);
	}

	
	if(framesOut == nullptr) {
		framesOut = (float*)malloc(sizeof(float)*(nFrames<<1));
		framesIn = (float*)malloc(sizeof(float)*(nFrames));
		framesXs = (float*)malloc(sizeof(float)*(nFrames)<<4);
		nNormal = jack->frames;
	}

	for(int i = 0; i < nFrames; i++)
		framesIn[i] = period[i];

	free(period);
	bufLock.unlock();

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
		out->frames = nNormal;
		if(out->feed(convPeriod) == FEED_OK)
			workState = READY;

		if(releasePeriod != nullptr && out->feed(releasePeriod) == FEED_WAIT)  {
			workState = RELEASE;
			cout << "Waiting to release" << endl;
		}
		else {
			cout << "Released" << endl;
			releasePeriod = nullptr;
		}

	} else
	if(workState == RELEASE) {
		Jack *out = getPlug("audio_out")->jack;
		if(out->feed(releasePeriod) == FEED_OK) {
			workState = READY;
			releasePeriod = nullptr;
			cout << "Released after wait" << endl;
		}
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
