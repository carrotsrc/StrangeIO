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
#ifndef RUIMPULSE_H
#define RUIMPULSE_H
#include "framework/rack/RackUnit.h"


namespace ExampleCode {
/** An impulse generator unit
 *
 * This is can be used to send an single impulse
 * down the rack every given period (measured in
 * milliseconds). The feedback also gives the 
 * number of samples.
 *
 * The unit assumes a single channel stream of
 * frames -- even if the output is interleaved -- 
 * so this should be taken into account when 
 * viewing the output
 *
 */
class RuImpulse : public RackoonIO::RackUnit
{
public:
	enum WorkState {
		IDLE,
		INIT,
		READY,
		WAITING
	};

	RuImpulse();
	RackoonIO::FeedState feed(RackoonIO::Jack*);
	void setConfig(string,string);

	RackoonIO::RackState init();
	RackoonIO::RackState cycle();
	void block(RackoonIO::Jack*);

private:
	WorkState workState;
	short mWait, mImpulseValue, *mFrames;
	int mSampleRate, mBlockSize, mSampleWait, mSampleCount;
	RackoonIO::Jack *mImpulseJack;

	void writeFrames();
};

}
#endif 
