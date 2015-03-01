#include <math.h>
#include "RuSine.h"
using namespace RackoonIO;
using namespace ExampleCode;
#define PHASE 0

RuSine::RuSine()
: RackUnit(std::string("RuSine")) {
	workState = IDLE;
	mBlockSize = 512;
	mAmplitude = 8000;
	mWaveSample = mWaveTime = 0.0f;
	mSampleRate = 44100;
	mF0 = mFreq = 220;
	mLambda = (float)1/mSampleRate;
	mModTime = 2.0f;
	mModPhase = true;
	mModSteps = ((int)(mSampleRate*mModTime)<<1);

	mPhaseF1 = mPhaseF0 = mFreq;

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
	CONSOLE_MSG("RuSine", "Frequence " << mFreq << " hz");
	CONSOLE_MSG("RuSine", "Amplitude " << mAmplitude);
	CONSOLE_MSG("RuSine", "Lambda " << mLambda << " seconds/sample");
	mSinewaveJack = getPlug("sinewave")->jack;
	mSinewaveJack->frames = mBlockSize;
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
	mPeriod = cacheAlloc(1);

	if(mModPhase)
		return modulatePhase();

	/* The output is interleaved so
	 * what we need to do is output the
	 * same value on both channels
	 */
	for(int i = 0; i < mBlockSize; i++) {
		short y = (short) mAmplitude * sin(2 * M_PI * mFreq * mWaveTime);
		mPeriod[i++] = y;
		mPeriod[i] = y;
		mWaveTime += mLambda;
	}
}

void RuSine::modulatePhase() {
	
	int i = 0;
	for(; i < mBlockSize; i++) {
		double delta = (double) mModCurrentStep / mModSteps;
		double t = (double) mModTime*delta;
		double phi = 2* M_PI *t*(mPhaseF0 + (mPhaseF1 - mPhaseF0)*delta / 2);
		short y = (short) (mAmplitude)*sin(phi);

		mPeriod[i++] = y;
		mPeriod[i] = y;

		mModCurrentStep += 2;
		mWaveTime += mLambda;

		if(mModCurrentStep == mModSteps) {
			mPhaseF0 = mPhaseF1;
			mModCurrentStep = 0;
		}
	}
}
/** This method can be bound to a MIDI controller
 */
void RuSine::midiFrequency(int value) {
	if(value == 64) {
		mPhaseF1 = mF0;
	} else
	if(value < 64) {
		mPhaseF1 = mF0 - (63-value);
	} else {
		mPhaseF1 = mF0 + ((value%64)*5);
		mModPhase = true;
		mModCurrentStep = 0;
	}

	cout << "F1: " << mPhaseF1 << endl;


}
