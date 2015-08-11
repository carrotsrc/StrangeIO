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

using namespace StrangeIO;
using namespace StrangeIO::Config;
using namespace StrangeIO::Testing;


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
