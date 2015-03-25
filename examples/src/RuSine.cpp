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
	mF1 = mF0 = mFreq = 220;
	mSamplePeriod = (float)1/mSampleRate;

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
	CONSOLE_MSG("RuSine", "Sample Period " << mSamplePeriod << " seconds/sample");
	mSinewaveJack = getPlug("sinewave")->jack;
	mSinewaveJack->frames = mBlockSize;
	mInstPhase = 0;
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
	/* The output is interleaved so
	 * what we need to do is output the
	 * same value on both channels
	 */
	mPeriod = cacheAlloc(1);
	for(int i = 0; i < mBlockSize; i++) {
		short y = (short) mAmplitude * sin(2 * M_PI * mF1 * mWaveTime + mInstPhase);
		mPeriod[i++] = y;
		mPeriod[i] = y;
		mWaveTime += mSamplePeriod;
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
	float oldFreq = mF1;

	if(value == 64) {
		mF1 = mF0;
	} else
	if(value < 64) {
		mF1 = mF0 - ((63-value)*2);
	} else {
		mF1 = mF0 + (((value-64))*2);
	}

	double c = fmod((mWaveTime*oldFreq+mInstPhase), (2*M_PI));
	double n = fmod((mWaveTime*mF1), (2*M_PI));
	cout << "C: "<< c << endl;
	mInstPhase = c-n;
	std::cout << "Target: " << mF1 << "Hz ";
	mRecombobulate.unlock();
}
