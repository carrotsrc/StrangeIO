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
#ifndef __TESTINGUNIT_H_1439314635__
#define __TESTINGUNIT_H_1439314635__
#include "framework/rack/RackUnit.h"
#include<atomic>
namespace strangeio {
namespace Testing {

#define CHANNEL_TWO 808.0f
#define SINGLE_CHANNEL 100.0f
#define DOUBLE_CHANNEL 101.0f
#define FEED_TEST 102.0f
#define CYCLE_TEST 103.0f
#define CONCURRENT_TEST 104.0f

#define CYCLE_SYNC 300u
#define CYCLE_CONCURRENT 301u
#define CYCLE_EVENTLOOP 302u

class TestingUnit : public strangeio::RackUnit
{
public:
	TestingUnit(std::string rutype) : strangeio::RackUnit(rutype) { };
	void setFeedCheck(int* feeder);
	void setCycleCheck(int* cycle);
	void toggleConcurrentTest(bool flag);

protected:
	int* mFeed, *mCycle;

	bool mConcurrent;
};

} // Testing
} // StrangeIO
#endif

