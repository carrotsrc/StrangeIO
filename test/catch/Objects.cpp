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
#include "framework/fwcommon.hpp"

#if !DEVBUILD
	#error The testing suite requires DEVBUILD to be enabled
#endif


#include "framework/component/Unit.hpp"
using namespace StrangeIO;

TEST_CASE( "StrangeIO::Component", "[StrangeIO::Component]" ) {

	SECTION("Unit") {
		Component::Unit unit(Component::UnitType::Mainliner, "SioTest", "test_unit");

		REQUIRE(unit.utype() == Component::UnitType::Mainliner);
		REQUIRE(unit.umodel() == "SioTest");
		REQUIRE(unit.ulabel() == "test_unit");
		REQUIRE(unit.cstate() == Component::ComponentState::Inactive);
	}

}
