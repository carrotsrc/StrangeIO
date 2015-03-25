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
	PcmSample *mPeriod;

	int mAmplitude, ///< Amplitude of wave
	    mBlockSize, ///< Period size in frames
	    mSampleRate; ///< Sample rate

	PcmSample mFreq, ///< Frequence of sine wave (hz)
	          mF0, ///< Centre frequency
		  mF1, ///< Current Freq
		  mFn, ///< Next frequency
	          mSamplePeriod, ///< Sample/rate ratio
	          mInstPhase;

	float  mWaveTime, ///< The wave time
	       mModTime, ///< Time to perform modulation
	       mWaveSample; ///< Current sample in the wave

	std::mutex mRecombobulate;

	void writeFrames();
	void modulatePhase();
	RackoonIO::Jack *mSinewaveJack; ///< Jack to push to
	void midiFrequency(int);
};

}
#endif 
