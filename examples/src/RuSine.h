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
		WAITING
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
	int mFreq, ///< Frequence of sine wave (hz)
	    mAmplitude, ///< Amplitude of wave
	    mBlockSize, ///< Period size in frames
	    mSampleRate; ///< Sample rate

	double mWaveTime, ///< The wave time
	      mLambda, ///< Sample/rate ration
	    mWaveSample; ///< Current sample in the wave
	void writeFrames();

	RackoonIO::Jack *mSinewaveJack; ///< Jack to push to
};

}
#endif 
