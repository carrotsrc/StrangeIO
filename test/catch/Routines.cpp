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
#include "framework/helpers/sound.h"

using namespace StrangeIO::Helpers::SoundRoutines;

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

#include "framework/helpers/midi.h"

using namespace StrangeIO::Helpers::MidiRoutines;

TEST_CASE( "Convert velocity to a normalised range", "[MidiRoutine]" ) {

	SECTION( "Normalise to a range of -1.0 to +1.0" ) {
		REQUIRE( normaliseVelocity64(127) == 1.0f );
		REQUIRE( normaliseVelocity64(64) == 0.0f );
		REQUIRE( normaliseVelocity64(0) == -1.0f );
	}

	SECTION( "Normalise to a range of 0.0 to 1.0" ) {
		REQUIRE( normaliseVelocity128(127) == 1.0f );
		REQUIRE( normaliseVelocity128(64) == 0.5f );
		REQUIRE( normaliseVelocity128(0) == 0.0f );
	}
}

