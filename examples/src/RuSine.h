#ifndef RUSINE_H
#define RUSINE_H
#include "framework/rack/RackUnit.h"
namespace ExampleCode {
/** Sine wave generator unit
 *
 * Produces a sine wave of a particular freq.
 * 
 * - Demonstrates exporting methods
 *   for binding to midi controllers
 *
 * For details on the different API methods,
 * see RuAlsa or RuImpulse
 */
class RuSine : public RackoonIO::RackUnit
{
public:
	enum WorkState {
		IDLE,
		INIT,
		READY,
		WAITING,
	};

	RuSine();
	RackoonIO::FeedState feed(RackoonIO::Jack*);
	void setConfig(string,string);

	RackoonIO::RackState init();
	RackoonIO::RackState cycle();
	void block(RackoonIO::Jack*);

private:
	WorkState workState;
	short *mPeriod;

	bool mModPhase;

	int mAmplitude, ///< Amplitude of wave
	    mBlockSize, ///< Period size in frames
	    mSampleRate; ///< Sample rate

	double mFreq, ///< Frequence of sine wave (hz)
	       mF0, ///< Cetnre frequency
	       mWaveTime, ///< The wave time
	       mLambda, ///< Sample/rate ratio
	       mInstPhase,
	       mWaveSample; ///< Current sample in the wave

	double mPhaseF0, ///< Start freq of modulation
	       mPhaseF1, ///< End freq of modulation
	       mModTime; ///< Time to perform modulation

	int mModSteps, ///< Number of sample to perform modulation
	    mModCurrentStep; ///< The current step through the modulation

	void writeFrames();
	void modulatePhase();

	RackoonIO::Jack *mSinewaveJack; ///< Jack to push to
	void midiFrequency(int);
};

}
#endif 
