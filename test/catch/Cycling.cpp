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
#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "framework/common.h"
#include "framework/rack/Rack.h"
#include "framework/rack/RackUnit.h"
#include "framework/rack/config/RackDocument.h"
#include "framework/rack/config/RackAssembler.h"
#include "units/TestingUnit.h"
#if !DEVBUILD
	#error The testing suite requires DEVBUILD to be enabled
#endif

using namespace strangeio;
using namespace strangeio::Config;
using namespace strangeio::Testing;


TEST_CASE( "Test Basic Feed", "[Feed]" ) {

	int feedCheck = 0;
	Config::RackAssembler as(std::unique_ptr<RackUnitGenericFactory>(new RackUnitGenericFactory()));
	auto unit = as.assembleUnit("MainlineUnit", "mainline", "units/MainlineUnit.rso");
	auto unitPtr = unit.get();

	Plug p(nullptr);
	SeqJack j("audio", unitPtr);
	p.jack = &j;
	p.unit = unitPtr;


	(static_cast<TestingUnit*>(unitPtr))->setFeedCheck(&feedCheck);

	SECTION ( "Single Channel" ) {
		j.numSamples = 1;
		j.numChannels = 1;
		PcmSample pcmFeed[1] = {SINGLE_CHANNEL};
		auto state = p.jack->feed(pcmFeed);
		REQUIRE(state == FEED_OK);
		REQUIRE(feedCheck == 1);
	}

	SECTION ( "Two Channel" ) {
		j.numSamples = 1;
		j.numChannels = 2;
		PcmSample pcmFeed[2] = {DOUBLE_CHANNEL,CHANNEL_TWO};
		auto state = p.jack->feed(pcmFeed);
		REQUIRE(state == FEED_OK);
		REQUIRE(feedCheck == 2);
	}

}

TEST_CASE( "Test Two Unit Feed", "[UnitsFeed]" ) {

	int feedCheck = 0;
	Config::RackAssembler as(std::unique_ptr<RackUnitGenericFactory>(new RackUnitGenericFactory()));
	auto unitMain = as.assembleUnit("MainlineUnit", "mainline", "units/MainlineUnit.rso");
	auto unitOut = as.assembleUnit("OutputUnit", "output", "units/OutputUnit.rso");
	auto ptrMain = unitMain.get();
	auto ptrOut = unitOut.get();

	Plug p(nullptr);
	p.jack = unitMain->getJack("power");
	p.jack->numSamples = 1;
	p.jack->numChannels = 1;
	p.unit = ptrMain;
	unitMain->setConnection("audio_out", "audio", ptrOut);

	PcmSample pcmFeed = FEED_TEST;


	(static_cast<TestingUnit*>(ptrMain))->setFeedCheck(&feedCheck);
	(static_cast<TestingUnit*>(ptrOut))->setFeedCheck(&feedCheck);
	SECTION ( "Testing feed" ) {
		auto state = p.jack->feed(&pcmFeed);
		REQUIRE(state == FEED_OK);
		REQUIRE(feedCheck == 2);
	}

}

TEST_CASE( "Rack Cycle", "[RackCycle]" ) {

	int feedCheck = 0;
	int cycleCheck = 0;

	Rack rack;
	Config::RackAssembler as(std::unique_ptr<RackUnitGenericFactory>(new RackUnitGenericFactory()));

	auto unitMain = as.assembleUnit("MainlineUnit", "mainline", "units/MainlineUnit.rso");
	auto unitOut = as.assembleUnit("OutputUnit", "output", "units/OutputUnit.rso");

	(static_cast<TestingUnit*>(unitMain.get()))->setFeedCheck(&feedCheck);
	(static_cast<TestingUnit*>(unitOut.get()))->setFeedCheck(&feedCheck);

	(static_cast<TestingUnit*>(unitMain.get()))->setCycleCheck(&cycleCheck);
	(static_cast<TestingUnit*>(unitOut.get()))->setCycleCheck(&cycleCheck);

	rack.addMainline("ac1");
	rack.addUnit(std::move(unitMain));
	rack.addUnit(std::move(unitOut));
	rack.connectUnits("rack", "ac1", "mainline", "power");
	rack.connectUnits("mainline", "audio_out", "output", "audio");

	SECTION ( "Test Cycle" ) {
		rack.exposeCycle(); // Warm up
		for(auto i = 0; i < 3; i++) {
			rack.exposeCycle();
		}
		REQUIRE(feedCheck == 3);
		REQUIRE(cycleCheck == 8);
	}

}

TEST_CASE( "Cycle with concurrent tasks", "[ConcurrentTasks]" ) {

	int feedCheck = 0;
	int cycleCheck = 0;

	Rack rack;
	rack.setRackQueue(std::unique_ptr<RackQueue>(new RackQueue(0)));
	auto queue = rack.getRackQueue();
	queue->setSize(3);
	queue->init();
	std::this_thread::sleep_for(std::chrono::milliseconds(20));

	Config::RackAssembler as(std::unique_ptr<RackUnitGenericFactory>(new RackUnitGenericFactory()));

	auto unitMain = as.assembleUnit("MainlineUnit", "mainline", "units/MainlineUnit.rso");
	auto unitOut = as.assembleUnit("OutputUnit", "output", "units/OutputUnit.rso");

	unitMain->setConfig("cycle_type", std::to_string(CYCLE_CONCURRENT));
	(static_cast<TestingUnit*>(unitMain.get()))->setFeedCheck(&feedCheck);
	(static_cast<TestingUnit*>(unitOut.get()))->setFeedCheck(&feedCheck);

	(static_cast<TestingUnit*>(unitMain.get()))->setCycleCheck(&cycleCheck);
	(static_cast<TestingUnit*>(unitOut.get()))->setCycleCheck(&cycleCheck);

	(static_cast<TestingUnit*>(unitMain.get()))->toggleConcurrentTest(true);
	(static_cast<TestingUnit*>(unitOut.get()))->toggleConcurrentTest(true);

	rack.addMainline("ac1");
	rack.addUnit(std::move(unitMain));
	rack.addUnit(std::move(unitOut));
	rack.connectUnits("rack", "ac1", "mainline", "power");
	rack.connectUnits("mainline", "audio_out", "output", "audio");

	SECTION ( "Test Cycle" ) {
		REQUIRE(queue->getSize() == 3);

		rack.exposeCycle(); // Warm up
		rack.exposeCycle(); // Actual
		std::this_thread::sleep_for(std::chrono::milliseconds(20));

		REQUIRE(feedCheck == 2);
		REQUIRE(cycleCheck == 4);
	}

}
