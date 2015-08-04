#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "framework/helpers/sound.h"

using namespace RackoonIO::Helpers::SoundRoutines;

TEST_CASE( "Interleaving routines of two channels", "[SoundRoutine]" ) {
	PcmSample interleaved[12] = { 
		0.50f, 0.10f,
		0.51f, 0.11f,
		0.52f, 0.12f,
		0.53f, 0.13f,
		0.54f, 0.14f,
		0.55f, 0.15f
		};

	PcmSample out[12] = {0};
	deinterleave2(interleaved, out, 6);

	SECTION( "Deinterleave a two channel period of samples" ) {

		REQUIRE( out[0] == 0.50f );
		REQUIRE( out[5] == 0.55f );
		REQUIRE( out[6] == 0.10f );
		REQUIRE( out[11] == 0.15f );
	}

	PcmSample reinter[12] = {0};
	interleave2(out, reinter, 6);

	SECTION( "Interleave a two channel period of samples" ) {

		REQUIRE( reinter[0] == 0.50f );
		REQUIRE( reinter[1] == 0.10f );

		REQUIRE( reinter[10] == 0.55f );
		REQUIRE( reinter[11] == 0.15f );
	}


}

