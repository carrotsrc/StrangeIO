#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "framework/common.h"
#include "framework/rack/Rack.h"
#include "framework/rack/RackUnit.h"
#include "framework/factories/RackUnitGenericFactory.h"
#include "framework/rack/config/RackDocument.h"
#include "framework/rack/config/RackAssembler.h"

#if !DEVBUILD
	#error The testing suite requires DEVBUILD to be enabled
#endif

using namespace StrangeIO;

TEST_CASE( "Dynamically load a unit", "[UnitLoading]" ) {
	Config::RackAssembler as(std::unique_ptr<RackUnitGenericFactory>(new RackUnitGenericFactory()));
	auto unit = as.assembleUnit("MainlineUnit", "mainline", "./units/MainlineUnit.rso");
	SECTION ( "Checking loaded unit" ) {
		REQUIRE( unit != nullptr );
		REQUIRE( unit->getRuType() == "MainlineUnit" );
		REQUIRE( unit->getName() == "mainline" );
	}

}
