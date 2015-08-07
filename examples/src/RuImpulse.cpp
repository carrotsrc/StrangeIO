/* Copyright 2015 Charlie Fyvie-Gauld
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published
 *  by the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "RuImpulse.h"
#include "framework/events/FrameworkMessages.h"
using namespace StrangeIO;
using namespace ExampleCode;

//** Initialise to default values */
RuImpulse::RuImpulse()
: RackUnit(std::string("RuImpulse")) {
	addPlug("impulse"); // This is the plug used to propogate the impulse down the chain
	addJack("power", JACK_AC); // This is the mainline jack, being a mainline unit
	workState = IDLE;
	mSampleRate = 44100;
	mWait = 500;
	mImpulseValue = 1800;
	mBlockSize = 512;
	mSampleCount = 0;
}

FeedState RuImpulse::feed(Jack *jack) {
	return FEED_OK;
}

/** set the config
 *
 * The config is done when the unit is first initialised
 * before being connect up in the daisychains.
 *
 * This unit accepts:
 *
 * - wait_time : The period between impulses (milliseconds)
 * - impulse_value : The value of the impulse
 *
 * @param config The config key
 * @param value The value to apply to the configuration
 */
void RuImpulse::setConfig(std::string config, std::string value) {
	if(config == "wait_time")
		mWait = atoi(value.c_str());
	else
	if(config == "impulse_value")
		mImpulseValue = atoi(value.c_str());
}

/** Initialise the unit
 *
 * This is called on the warm up cycle of the Rack.
 *
 * Here is where we need to set any variables that are
 * needed in order for the unit to process correctly.
 *
 * @return RACK_UNIT_OK to say that the unit has initialised OK; other wise RACK_UNIT_FAILURE
 */
RackState RuImpulse::init() {

	// How many samples do we wait for?
	mSampleWait = (int)((44100/1000)*mWait)<<1;

	// Here we are printing useful information out to the console
	UnitMsg("Time: " << (mWait) << " ms");
	UnitMsg("Value: " << mImpulseValue);
	UnitMsg("Samples: " << mSampleWait);

	mImpulseJack = getPlug("impulse")->jack;
	mImpulseJack->frames = mBlockSize;

	UnitMsg("Initialised");
	workState = READY;
	return RACK_UNIT_OK;
}

/** Called every rack cycle
 *
 * This unit doesn't need to outsource tasks
 * so it does all processing on the AC signal
 * sent by the rack.
 *
 * @return If everything is fine - RACK_UNIT_OK; otherwise RACK_UNIT_FAILURE
 */
RackState RuImpulse::cycle() {

	/* If we're READY, then the
	 * unit can continue writing
	 * frames
	 */
	if(workState == READY)
		writeFrames();

	/* Here we switch the state machine depending on
	 * whether downstream can take any more frames or
	 * has asked that we wait.
	 *
	 * if we wait, we change our state to WAITING
	 * and retry with the current period that is stored
	 */
	workState =
	(mImpulseJack->feed(mFrames) == FEED_OK)
	? READY : WAITING;

	return RACK_UNIT_OK;
}

void RuImpulse::block(Jack *jack) {
	/* this is a mainline unit
	 * so won't receive any block
	 * signals from upstream
	 */
}

/** Custom method to write either zeros or impulse
 *
 * If the unit has fed enough zeros, it can then
 * send an impulse down the line.
 */
void RuImpulse::writeFrames() {
	mFrames = cacheAlloc(1);
	mSampleCount += mBlockSize;
	memset(mFrames, 0, mBlockSize*sizeof(PcmSample));

	if( mSampleCount > mSampleWait ) {
		int diff = mSampleCount - mSampleWait - 1;
		mFrames[diff] = mImpulseValue;
		mSampleCount = 0;
	}
	notifyProcComplete();
}
