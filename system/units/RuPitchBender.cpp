#include "RuPitchBender.h"

using namespace RackoonIO;

RuPitchBender::RuPitchBender() : RackUnit() { addJack("audio", JACK_SEQ);
	addPlug("audio_out"); 
	workState = IDLE; 
	framesIn = framesOut = nullptr;
	ratio = 1.0; 
	convPeriod = nullptr;
	resampler = nullptr; 
	releasePeriod = nullptr;
	nRemainder = 0;
	MIDI_BIND("pitchBend", RuPitchBender::midiBend); 
	fp = fopen("fp1.raw", "wb");
	fp2 = fopen("fp2.raw", "wb");
}

RuPitchBender::~RuPitchBender() {

	if(framesIn != nullptr) { 
		free(framesIn); 
		free(framesOut); 
	} 
}

void RuPitchBender::overwritePeriod(short *dst, int value, int count) {
	for(int i = 0; i < count; i++)
		dst[i] = value;
}

void RuPitchBender::actionResample() {
	bufLock.lock();
	int usedFrames;
	convPeriod = (short*)calloc(nNormal, sizeof(short));
	if(nRemainder) {
		if(nRemainder <= nNormal) {
			if(nFrames) {
				int i;
				for(i = 0; i < nRemainder; i++)
					*(convPeriod+i) = (short)*(remRead+i);
				remRead = remainder;
			}
		}
		else {
			for(int i = 0; i < nNormal; i++)
				*(convPeriod+i) = (short)*(remRead+i);

			remRead += nNormal;
			nRemainder -= nNormal;
			bufLock.unlock();
			workState = FLUSH_REMAINDER;
			return;
		}
	}

	if(nRemainder == nNormal) {
		nRemainder = 0;
		bufLock.unlock();
		workState = FLUSH_REMAINDER;
		return;
	}

	nResampled = resample_process(resampler, ratio, framesIn, nFrames, 0, &usedFrames,
					framesOut, nFrames<<1);
	if((nResampled+nRemainder) >= nNormal) {
		// get normalized period and store the remainder
		int i;
		for(i = 0; i < nNormal-nRemainder; i++)
			*(convPeriod+nRemainder+i) = (short)*(framesOut+i);

		int oldRem = nRemainder;
		nRemainder = (nResampled+nRemainder-nNormal);
		memcpy(remainder, framesOut+(nNormal-oldRem), nRemainder*sizeof(float));
		workState = FLUSH;

	} else {
		memcpy(remainder+nRemainder, framesOut, nResampled*sizeof(float));
		nRemainder = nResampled+nRemainder;
		workState = WAITING;
	}

	bufLock.unlock();

}

inline void RuPitchBender::sfMemcpy(float *dst, short *src, int size) {
	for(int i = 0; i < size; i++)
		dst[i] = (float) src[i];
}

inline void RuPitchBender::fsMemcpy(short *dst, float *src, int size) {
	for(int i = 0; i < size; i++)
		dst[i] = (short) src[i];
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
	dd = false;
	if(workState == WAITING){
		dd = true;
	}
	sfMemcpy(framesIn, period, nFrames);
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
	if(workState == FLUSH || workState == FLUSH_REMAINDER) {
		Jack *out = getPlug("audio_out")->jack;
		out->frames = nNormal;
		if(out->feed(convPeriod) == FEED_OK) {
			if(workState == FLUSH_REMAINDER) {
				OUTSRC(RuPitchBender::actionResample);
				workState = REMAINDER_WAITING;
			}
			else
				workState = READY;
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
