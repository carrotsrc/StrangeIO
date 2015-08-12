/* Copyright (C) 2015  Charlie Fyvie-Gauld
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "MainlineUnit.h"

using namespace StrangeIO;
using namespace StrangeIO::Testing;

MainlineUnit::MainlineUnit()
: TestingUnit(std::string("MainlineUnit")) {
	workState = IDLE;
	addPlug("audio_out");
	addJack("power", JACK_AC, 1);
	MidiExport("exported", MainlineUnit::exportedMethod);
	mFeed = nullptr;
	mCycleType = CYCLE_SYNC;
}

FeedState MainlineUnit::feed(Jack *jack) {
	if(mFeed == nullptr)
		return FEED_WAIT;

	(*mFeed)++;
	PcmSample* period;
	jack->flush(&period,1);
	auto out = getPlug("audio_out")->jack;

	auto val = static_cast<int>(*period);
	switch(val) {

	case SINGLE_CHANNEL:
		return FEED_OK;
		break;

	case DOUBLE_CHANNEL:
		jack->flush(&period,2);

		if(*period != CHANNEL_TWO) {
			return FEED_WAIT;
		}

		(*mFeed)++;
		return FEED_OK;
		break;

	case FEED_TEST:
		out->numSamples = jack->numSamples;
		out->numChannels = jack->numChannels;
		return out->feed(period);
		break;

	default:
		return FEED_WAIT;
	}

	return FEED_WAIT;
}
void MainlineUnit::setConfig(std::string config, std::string value) {
	if(config == "cycle_type") {
		mCycleType = stoi(value);
	} else if( config == "create_listener" ) {
		mEventType = stoi(value);
		EventListener((EventType)mEventType, MainlineUnit::eventCallback);
	}
}

RackState MainlineUnit::init() {
	(*mCycle)++;
	workState = READY;
	return RACK_UNIT_OK;
}

RackState MainlineUnit::cycle() {
	(*mCycle)++;
	auto period = new PcmSample[1];
	switch(mCycleType) {
	case CYCLE_SYNC: period[0] = {CYCLE_TEST}; break;
	case CYCLE_CONCURRENT: period[0] = {CONCURRENT_TEST}; break;
	case CYCLE_EVENTLOOP: addEvent(createMessage(mEventType)); return RACK_UNIT_OK;
	default: period[0] = {CYCLE_TEST}; break;
	}
	auto out = getPlug("audio_out")->jack;
	out->numChannels = 1;
	out->numSamples = 1;
	out->feed(period);
	return RACK_UNIT_OK;
}

void MainlineUnit::block(Jack *jack) {
}

void MainlineUnit::exportedMethod(int value) {

}
void MainlineUnit::eventCallback(std::shared_ptr<StrangeIO::EventMessage> msg) {
	(*mCycle) = msg->msgType;
}

DynamicBuilder(MainlineUnit);
