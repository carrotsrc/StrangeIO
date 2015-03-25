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
		BUSY
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

	int mAmplitude, ///< Amplitude of wave
	    mBlockSize, ///< Period size in frames
	    mSampleRate; ///< Sample rate

	double mFreq, ///< Frequence of sine wave (hz)
	       mF0, ///< Cetnre frequency
	       mWaveTime, ///< The wave time
	       mSamplePeriod, ///< Sample/rate ratio
	       mInstPhase,
	       mWaveSample; ///< Current sample in the wave

	double mF1, ///< Next frequency
	       mModTime; ///< Time to perform modulation

	std::mutex mRecombobulate;

	void writeFrames();
	void modulatePhase();
	RackoonIO::Jack *mSinewaveJack; ///< Jack to push to
	void midiFrequency(int);
};

}
#endif 
