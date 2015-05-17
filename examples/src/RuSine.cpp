#include <math.h>
#include "RuSine.h"
using namespace RackoonIO;
#define PHASE 0

RuSine::RuSine()
: RackUnit(std::string("RuSine")) {
	workState = IDLE;
	mBlockSize = 512;
	mFs = 44100;
	mF1 = 0;
	mFn = mF0 = 220;
	mInstPhase = mLastPhase = 0;

	addPlug("sinewave");
	addJack("power", JACK_AC);

	/* We use this method to export a method
	 * representing the action 'freq' so it
	 * can be bound to a MIDI controller
	 * in the configuration file
	 */
	MidiExport("freq", RuSine::midiFrequency);
}

FeedState RuSine::feed(Jack *jack) {
	return FEED_OK;
}

void RuSine::setConfig(string config, string value) { 
	// configuration sets centre frequency
	if(config == "freq") {
		mFn = mF0 = atoi(value.c_str());
	}

}

RackState RuSine::init() {
	UnitMsg("F0: " << mF0 << " hz");
	mSinewaveJack = getPlug("sinewave")->jack;
	mSinewaveJack->frames = mBlockSize;
	m2Pi = 2*M_PI;
	workState = READY;
	UnitMsg("Initialised");
	return RACK_UNIT_OK;
}

RackState RuSine::cycle() {
	if(workState == READY) writeSamples();

	workState = (mSinewaveJack->feed(mPeriod) == FEED_OK)
		? READY : WAITING;
	return RACK_UNIT_OK;
}

void RuSine::block(Jack *jack) {
}

void RuSine::writeSamples() {
	mPeriod = cacheAlloc(1); // Get a fresh block

	/* mFn is atomic so is a thread safe read
	 * to the local variable.
	 */
	float F = mFn.load();

	if(mF1 != F) {
		/* Recalculate delta for every 
		 * frequency change
		 */
		mF1 = F;
		mDelta = (float)(m2Pi*mF1)/mFs;
	}

	for(int i = 0; i < mBlockSize; i++) {
		mInstPhase = fmod((mLastPhase+mDelta), m2Pi);
		mLastPhase = mInstPhase;
		PcmSample y = (PcmSample) sin(mInstPhase);

		/* The output is interleaved so
		 * what we need to do is output the
		 * same value on both channels
		 */
		mPeriod[i++] = y;
		mPeriod[i] = y;
	}
}

/** This method can be bound to a MIDI controller
 */
void RuSine::midiFrequency(int value) {

	if(value == 64) {
		mFn.store(mF0);
	} else
	if(value < 64) {
		mFn.store(mF0 - ((63-value)*2));
	} else {
		mFn.store(mF0 + ((value%64)*2));
	}

}

// Make this unit loadable at runtime by defining a builder method
DynamicBuilder(RuSine);

