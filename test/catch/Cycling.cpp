#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "framework/common.h"
#include "framework/rack/Rack.h"
#include "framework/rack/RackUnit.h"
#include "framework/rack/config/RackDocument.h"
#include "framework/rack/config/RackAssembler.h"

#if !DEVBUILD
	#error The testing suite requires DEVBUILD to be enabled
#endif

using namespace StrangeIO::Config;

TEST_CASE( "Test UnitConnectors", "[UnitConnectors]" ) {
	RackDocument doc;

	auto config = doc.load("basic.cfg");

	SECTION ( "Checking system" ) {
		REQUIRE( config->system.threads.num_workers == 3 );
	}

}

TEST_CASE( "Test Unit Cycle", "[UnitCycling]" ) {
	RackDocument doc;

	auto config = doc.load("basic.cfg");

	SECTION ( "Checking system" ) {
		REQUIRE( config->system.threads.num_workers == 3 );
	}

}
