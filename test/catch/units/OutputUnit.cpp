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
#include "OutputUnit.h"

using namespace strangeio;

OutputUnit::OutputUnit()
: TestingUnit(std::string("OutputUnit")) {
	workState = IDLE;
	addJack("audio", JACK_SEQ, 1);
	mFeed = nullptr;
}

FeedState OutputUnit::feed(Jack *jack) {
	if(mFeed == nullptr)
		return FEED_WAIT;

	(*mFeed)++;

	PcmSample* period;
	jack->flush(&period,1);
	auto val = static_cast<int>(*period);
	switch(val) {
	case FEED_TEST:
		return FEED_OK;
	case CYCLE_TEST:
		free(period);
		return FEED_OK;
	case CONCURRENT_TEST:
		ConcurrentTask(OutputUnit::task);
		free(period);
		return FEED_OK;
	}

	return FEED_WAIT;

}

void OutputUnit::setConfig(std::string config, std::string value) {
}

RackState OutputUnit::init() {
	(*mCycle)++;
	workState = READY;
	return RACK_UNIT_OK;
}

RackState OutputUnit::cycle() {
	(*mCycle)++;
	return RACK_UNIT_OK;
}

void OutputUnit::block(Jack *jack) {
}

void OutputUnit::task() {
	(*mFeed)++;
}

DynamicBuilder(OutputUnit);
