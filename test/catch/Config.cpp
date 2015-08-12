#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "framework/common.h"
#include "framework/rack/config/RackDocument.h"

#if !DEVBUILD
	#error The testing suite requires DEVBUILD to be enabled
#endif

using namespace StrangeIO::Config;

TEST_CASE( "Load a configuration document", "[ConfigDocument]" ) {
	RackDocument doc;

	auto config = doc.load("basic.cfg");

	SECTION ( "Checking system" ) {
		REQUIRE( config->system.threads.num_workers == 3 );
	}

	SECTION ( "Checking mainlines" ) {
		REQUIRE( config->setup.mainlines.size()		== 1 );
		REQUIRE( config->setup.mainlines[0]			== "ac1" );
	}

	SECTION ( "Checking midi devices" ) {
		REQUIRE( config->midi.controllers.size()		== 1 );
		REQUIRE( config->midi.controllers[0].label		== "LaunchControl" );
		REQUIRE( config->midi.controllers[0].port		== "hw:1,0,0" );
	}

	SECTION ( "Checking daisychains" ) {
		REQUIRE( config->setup.daisychains.size() == 2 );

		REQUIRE( config->setup.daisychains[0].from	== "rack" );
		REQUIRE( config->setup.daisychains[0].plug	== "ac1" );
		REQUIRE( config->setup.daisychains[0].to	== "main" );
		REQUIRE( config->setup.daisychains[0].jack	== "power" );

		REQUIRE( config->setup.daisychains[1].from	== "main" );
		REQUIRE( config->setup.daisychains[1].plug	== "audio_out" );
		REQUIRE( config->setup.daisychains[1].to	== "masterout" );
		REQUIRE( config->setup.daisychains[1].jack	== "audio" );
	}

	SECTION ( "Checking units" ) {
		REQUIRE( config->setup.units.size() == 2 );

		REQUIRE( config->setup.units[0].label				== "main" );
		REQUIRE( config->setup.units[0].unit				== "MainlineUnit" );
		REQUIRE( config->setup.units[0].library				== "./units/MainlineUnit.rso" );
		REQUIRE( config->setup.units[0].configs.size()		== 1 );
		REQUIRE( config->setup.units[0].configs[0].type		== "test_config" );
		REQUIRE( config->setup.units[0].configs[0].value	== "test_value" );
		REQUIRE( config->setup.units[0].bindings.size()		== 1 );
		REQUIRE( config->setup.units[0].bindings[0].name	== "exported" );
		REQUIRE( config->setup.units[0].bindings[0].module	== "LaunchControl" );
		REQUIRE( config->setup.units[0].bindings[0].code	== 73 );

		REQUIRE( config->setup.units[1].label				== "masterout" );
		REQUIRE( config->setup.units[1].unit				== "OutputUnit" );
		REQUIRE( config->setup.units[1].library				== "./units/OutputUnit.rso" );
		REQUIRE( config->setup.units[1].configs.size()		== 3 );
		REQUIRE( config->setup.units[1].configs[2].type		== "unit_buffer" );
		REQUIRE( config->setup.units[1].configs[2].value	== "4096" );
		REQUIRE( config->setup.units[1].bindings.size()		== 0 );
	}
}

#include "framework/rack/Rack.h"
#include "framework/rack/config/RackAssembler.h"
using namespace StrangeIO;


TEST_CASE( "Assemble rack from configuration", "[ConfigAssembly]" ) {

	RackDocument doc;
	RackAssembler as(std::unique_ptr<RackUnitGenericFactory>(new RackUnitGenericFactory));
	Rack rack;
	rack.setRackQueue(std::unique_ptr<RackQueue>(new RackQueue(0)));

	const auto config = doc.load("basic.cfg");
	as.assemble((*config), rack);

	

	SECTION( "Checking number of workers" ) {
		auto queue = rack.getRackQueue();
		REQUIRE( queue->getSize() == config->system.threads.num_workers);
	}

	SECTION( "Checking MIDI devices" ) {
		auto& midi = rack.getMidiHandler();
		const auto& devices = midi.getModules();
		REQUIRE( devices.size()			== 1);
		REQUIRE( devices[0]->getAlias() == "LaunchControl" );

		const auto& bindings = devices[0]->getBindings();
		REQUIRE( bindings.find(73)		!= bindings.end() );
	}

	SECTION( "Checking units in rack" ) {
		REQUIRE( rack.hasUnit("main") );
		REQUIRE( rack.hasUnit("masterout") );
	}

	SECTION( "Checking specific unit" ) {
		auto u = rack.getUnit("main");
		REQUIRE( u->getName()			== "main" );

		auto jacks = u->exposeJacks();
		REQUIRE( jacks.size()			== 1 );
		REQUIRE( jacks[0]->name			== "power" );

		auto plugs = u->exposePlugs();
		REQUIRE( plugs.size() 			== 1 );
		REQUIRE( plugs[0]->name 		== "audio_out" );
		
		auto wptr = u->getRackQueue();
		REQUIRE( wptr.expired() 		== false);
	}

	SECTION( "Checking daisychain" ) {
		auto mainline = rack.getPlug("ac1");

		REQUIRE( mainline->name			== "ac1" );
		REQUIRE( mainline->connected	== true );

		auto jack = mainline->jack;
		REQUIRE( jack->name				== "power" );
		REQUIRE( jack->connected		== true);

		auto unit = mainline->unit;
		REQUIRE( unit					!= nullptr );
		REQUIRE( unit->getName()		== "main" );

		auto plugs = unit->exposePlugs();
		REQUIRE( plugs[0]->connected	== true );

		unit = plugs[0]->unit;
		REQUIRE( unit					!= nullptr );
		REQUIRE( unit->getName()		== "masterout" );
	}
	
}

