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
	int usedFrames;
	convPeriod = (short*)malloc(sizeof(short)*nNormal);
	cout << nRemainder << endl;
	if(nRemainder) {
		if(nRemainder <= nNormal) {
			fsMemcpy(remRead, convPeriod, nRemainder);
			remRead = remainder;
		}
		else {
			fsMemcpy(remRead, convPeriod, nNormal);
			remRead += nNormal;
			nRemainder -= nNormal;
			workState = FLUSH;
			OUTSRC(RuPitchBender::actionResample);
			bufLock.unlock();
			return;
		}
	}

	if(nRemainder == nNormal) {
		workState = FLUSH;
		OUTSRC(RuPitchBender::actionResample);
		nRemainder = 0;
		bufLock.unlock();
		return;
	}

	nResampled = resample_process(resampler, ratio, framesIn, nFrames, 0, &usedFrames,
					framesOut, nFrames<<1);
	if(nResampled > nNormal) {
		// get normalized period and store the remainder
		fsMemcpy(framesOut, convPeriod+nRemainder, nNormal-nRemainder);
		nRemainder = (nResampled+nRemainder-nNormal);
		memcpy(remainder, framesOut+nNormal+nRemainder, nRemainder);
		workState = FLUSHING;
		cout << "Flushing" << endl;
	} else {
		memcpy(remainder, framesOut, nFrames);
		workState = WAITING;
		cout << "Waiting" << endl;
	}

	bufLock.unlock();

}

inline void RuPitchBender::fsMemcpy(float *in, short *out, int size) {
	for(int i = 0; i < size; i++)
		out[i] = (short) in[i];
}

inline void RuPitchBender::sfMemcpy(short *in, float *out, int size) {
	for(int i = 0; i < size; i++)
		out[i] = (float) in[i];
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
		remainder = (float*)malloc(sizeof(float)*(nFrames)<<4);
		nNormal = jack->frames;
		remRead = remainder;
	}

	sfMemcpy(period, framesIn, nFrames);
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
	if(workState == FLUSHING || workState == FLUSH_REMAINDER) {
		Jack *out = getPlug("audio_out")->jack;
		out->frames = nNormal;
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
