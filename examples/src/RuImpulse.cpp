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
using namespace RackoonIO;
RuImpulse::RuImpulse()
: RackUnit(std::string("RuImpulse")) {
	addPlug("impulse");
	addJack("power", JACK_AC);
	workState = IDLE;
	mSampleRate = 44100;
	mWait = 500;
	mImpulseValue = 1800;
	mBlockSize = 512;
	mSampleCount = 0;
}

FeedState RuImpulse::feed(Jack *jack) {
	Jack *j = getJack("audio");
	return FEED_OK;
}

void RuImpulse::setConfig(string config, string value) {
	if(config == "wait_time")
		mWait = atoi(value.c_str());
	else
	if(config == "impulse_value")
		mImpulseValue = atoi(value.c_str());
}

RackState RuImpulse::init() {

	// How many samples do we wait for?
	int mSampleWait = (44100/1000)*mWait;
	mImpulseJack = getPlug("impulse")->jack;
	mImpulseJack->frames = mBlockSize;
	CONSOLE_MSG("RuImpulse", "Initialiased");
	workState = READY;
	return RACK_UNIT_OK;
}

void RuImpulse::writeFrames() {
	mFrames = cacheAlloc(1);
	mSampleCount += mBlockSize;
	memset(mFrames, 0, mBlockSize*sizeof(short));

	if( mSampleCount > mSampleWait ) {
		int diff = mSampleCount - mSampleWait;
		mFrames[mSampleCount-mSampleWait] = mImpulseValue; 
	}
}

RackState RuImpulse::cycle() {

	if(workState == READY)
		writeFrames();

	if(mImpulseJack->feed(mFrames) == FEED_OK)
		workState = READY;
	else
		workState = WAITING;

	return RACK_UNIT_OK;
}

void RuImpulse::block(Jack *jack) {
	
}
