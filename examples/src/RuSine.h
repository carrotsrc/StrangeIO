#ifndef RUSINE_H
#define RUSINE_H
#include <atomic>
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
	PcmSample *mPeriod;

	int mBlockSize, ///< Period size in frames
	    mFs, ///< Sample rate
	    mF0, ///< Centre frequency
	    mF1; ///< Current freqency

	std::atomic<int> mFn; ///< Next frequency

	float mLastPhase, ///< Previous phase for accumulator
	      mInstPhase, ///< Current instantaneous phase
	      mDelta, ///< angular frequency / sample frequency

	      m2Pi; ///< Value of 2PI


	std::mutex mRecombobulate;

	void writeSamples();
	RackoonIO::Jack *mSinewaveJack; ///< Jack to push to
	void midiFrequency(int);
};

}
#endif 
