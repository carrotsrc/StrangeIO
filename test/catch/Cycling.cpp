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


TEST_CASE( "Test Feed", "[UnitConnectors]" ) {

	int feedCheck = 0;
	Config::RackAssembler as(std::unique_ptr<RackUnitGenericFactory>(new RackUnitGenericFactory()));
	auto unit = as.assembleUnit("MainlineUnit", "mainline", "units/MainlineUnit.rso");
	auto unitPtr = unit.get();

	Plug p(nullptr);
	SeqJack j("audio", unitPtr);
	p.jack = &j;
	p.unit = unitPtr;


	(static_cast<TestingUnit*>(unitPtr))->setFeedCheck(&feedCheck);
	SECTION ( "Testing feed" ) {
		auto state = p.jack->feed(nullptr);
		REQUIRE(state == FEED_OK);
		REQUIRE(feedCheck == 1);
	}

}

TEST_CASE( "Test Unit Cycle", "[UnitCycling]" ) {

}
