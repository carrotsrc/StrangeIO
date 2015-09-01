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
#include "framework/factories/RackUnitGenericFactory.h"
#include "framework/rack/config/RackDocument.h"
#include "framework/rack/config/RackAssembler.h"

#if !DEVBUILD
	#error The testing suite requires DEVBUILD to be enabled
#endif

using namespace strangeio;

TEST_CASE( "Dynamically load a unit", "[UnitLoading]" ) {
	Config::RackAssembler as(std::unique_ptr<RackUnitGenericFactory>(new RackUnitGenericFactory()));
	auto unit = as.assembleUnit("MainlineUnit", "mainline", "./units/MainlineUnit.rso");
	SECTION ( "Checking loaded unit" ) {
		REQUIRE( unit != nullptr );
		REQUIRE( unit->getRuType() == "MainlineUnit" );
		REQUIRE( unit->getName() == "mainline" );
	}

}
