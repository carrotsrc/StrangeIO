#include <math.h>
#include "RuSine.h"
using namespace RackoonIO;
using namespace ExampleCode;
#define PHASE 0

RuSine::RuSine()
: RackUnit(std::string("RuSine")) {
	workState = IDLE;
	mBlockSize = 512;
	mWaveSample = mWaveTime = 0.0f;
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
	MIDI_BIND("freq", RuSine::midiFrequency);
}

FeedState RuSine::feed(Jack *jack) {
	return FEED_OK;
}

void RuSine::setConfig(string config, string value) {
}

RackState RuSine::init() {
	CONSOLE_MSG("RuSine", "Frequence " << mF0 << " hz");
	mSinewaveJack = getPlug("sinewave")->jack;
	mSinewaveJack->frames = mBlockSize;
	m2Pi = 2*M_PI;
	workState = READY;
	CONSOLE_MSG("RuSine", "Initialised");
	return RACK_UNIT_OK;
}

RackState RuSine::cycle() {
	if(workState == READY) writeFrames();

	workState = (mSinewaveJack->feed(mPeriod) == FEED_OK)
		? READY : WAITING;
	return RACK_UNIT_OK;
}

void RuSine::block(Jack *jack) {
}

void RuSine::writeFrames() {
	mRecombobulate.lock();
	mPeriod = cacheAlloc(1); // Get a fresh block


	if(mF1 != mFn) {
		/* Recalculate delta for every 
		 * frequency change
		 */
		mF1 = mFn;
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
	mRecombobulate.unlock();
}

/* Serious chirp problems */
void RuSine::modulatePhase() {

}
/** This method can be bound to a MIDI controller
 */
void RuSine::midiFrequency(int value) {
	mRecombobulate.lock();

	if(value == 64) {
		mFn = mF0;
	} else
	if(value < 64) {
		mFn = mF0 - ((63-value)*2);
	} else {
		mFn = mF0 + ((value%64)*2);
	}

	mRecombobulate.unlock();
}
