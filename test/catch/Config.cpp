#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "framework/rack/config/RackDocument.h"

using namespace StrangeIO::Config;

TEST_CASE( "Load a configuration document", "[ConfigDocument]" ) {
	RackDocument doc;

	auto config = doc.load("basic.cfg");

	SECTION ( "Checking system" ) {
		REQUIRE( config->system.threads.num_workers == 3 );
	}

	SECTION ( "Checking mainlines" ) {
		REQUIRE( config->setup.mainlines.size() == 1 );
		REQUIRE( config->setup.mainlines[0] == "ac1" );
	}

	SECTION ( "Checking midi devices" ) {
		REQUIRE( config->midi.controllers.size() == 1 );
		REQUIRE( config->midi.controllers[0].label == "LaunchControl" );
		REQUIRE( config->midi.controllers[0].port == "hw:1,0,0" );
	}

	SECTION ( "Checking daisychains" ) {
		REQUIRE( config->setup.daisychains.size() == 2 );

		REQUIRE( config->setup.daisychains[0].from == "rack" );
		REQUIRE( config->setup.daisychains[0].plug == "ac1" );
		REQUIRE( config->setup.daisychains[0].to == "flac1" );
		REQUIRE( config->setup.daisychains[0].jack == "power" );

		REQUIRE( config->setup.daisychains[1].from == "flac1" );
		REQUIRE( config->setup.daisychains[1].plug == "audio_out" );
		REQUIRE( config->setup.daisychains[1].to == "masterout" );
		REQUIRE( config->setup.daisychains[1].jack == "audio" );
	}

	SECTION ( "Checking units" ) {
		REQUIRE( config->setup.units.size() == 2 );

		REQUIRE( config->setup.units[0].label == "flac1" );
		REQUIRE( config->setup.units[0].unit == "SuFlacLoad" );
		REQUIRE( config->setup.units[0].library == "./units/SuFlacLoad.rso" );
		REQUIRE( config->setup.units[0].configs.size() == 1 );
		REQUIRE( config->setup.units[0].configs[0].type == "filename" );
		REQUIRE( config->setup.units[0].configs[0].value == "test.flac" );
		REQUIRE( config->setup.units[0].bindings.size() == 1 );
		REQUIRE( config->setup.units[0].bindings[0].name == "pause" );
		REQUIRE( config->setup.units[0].bindings[0].module == "LaunchControl" );
		REQUIRE( config->setup.units[0].bindings[0].code == 73 );
/*
		REQUIRE( config->setup.daisychains[0].to == "flac1" );
		REQUIRE( config->setup.daisychains[0].jack == "power" );
*/
	}
}
