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
#include "Objects.hpp"

#if !DEVBUILD
	#error The testing suite requires DEVBUILD to be enabled
#endif

#include "framework/component/Rack.hpp"
using namespace StrangeIO;

TEST_CASE( "Unit", "StrangeIO::Component" ) {

		OmegaUnit unit("Omega1");
		LinkIn lin {
			.label = "nowhere",
			.id = 0xf,
			.unit = nullptr
		};

		SECTION("Verify unit attributes") {
			REQUIRE(unit.utype() == UnitType::Mainliner);
			REQUIRE(unit.umodel() == "Omega");
			REQUIRE(unit.ulabel() == "Omega1");
		}

		SECTION("Test Warmup cycle and state change") {
			REQUIRE(unit.cstate() == ComponentState::Inactive);
			REQUIRE(unit.init_count() == 0); 

			REQUIRE(unit.cycle_line(CycleType::Warmup) == CycleState::Complete); 
			REQUIRE(unit.cstate() == ComponentState::Active);
			REQUIRE(unit.init_count() == 1);

			unit.cycle_line(CycleType::Warmup);
			REQUIRE(unit.init_count() == 1);
		}

		SECTION("Test Ac cycle") {
			REQUIRE(unit.cycle_line(CycleType::Ac) == CycleState::Complete); 
		}

		SECTION("Test Sync cycle") {
			REQUIRE(unit.cycle_line(CycleType::Sync) == CycleState::Complete); 
		}

		SECTION("Verify unit's profile") {
			unit.cycle_line(CycleType::Warmup);
			auto unit_profile = unit.unit_profile();
			REQUIRE(unit_profile.fs == 44100);
			REQUIRE(unit_profile.channels == 2);
			REQUIRE(unit_profile.latency == 1);
			REQUIRE(unit_profile.period == 1024);
		}


		SECTION("Veryify non-existant links") {
			REQUIRE(unit.has_input("foo") == -1);
			REQUIRE(unit.has_output("foo") == -1);

			REQUIRE(unit.get_input(0) == nullptr);
			REQUIRE(unit.get_output(0) == nullptr);
		}

		SECTION("Verify faulty connections") {
			REQUIRE(unit.connect(0, nullptr) == false);
			REQUIRE(unit.connect(0, &lin) == false);

			unit.delayed_constructor();
			REQUIRE(unit.connect(0, nullptr) == false);
		}


		SECTION("Verify input/output links and connect") {
			unit.delayed_constructor();
			REQUIRE(unit.has_input("power") != -1);
			REQUIRE(unit.has_output("audio") != -1);
			REQUIRE(unit.connect(0, &lin) == true);
		}

		SECTION("Verify connection state") {
			unit.delayed_constructor();
			unit.connect(0, &lin);
			auto out = unit.get_output(0);
			REQUIRE(out->label == "audio");
			REQUIRE(out->connected == true);
			REQUIRE(out->to->label == "nowhere");
			REQUIRE(out->to->unit == nullptr);
		}

		SECTION("Verify disconnect") {
			unit.delayed_constructor();
			unit.connect(0, &lin);
			unit.disconnect(0);
			auto out = unit.get_output(0);
			REQUIRE(out->connected == false);
			REQUIRE(out->to == nullptr);

		}


		SECTION("Verify Sync line profile") {
			unit.cycle_line(CycleType::Warmup);
			Profile profile { 0 };
			Profile& p = profile;

			unit.sync_line(p);
			REQUIRE(profile.latency == 1);
			REQUIRE(profile.channels == 2);
			REQUIRE(profile.fs == 44100);
			REQUIRE(profile.period == 1024);
			REQUIRE(profile.drift == 0.10f);
			REQUIRE(profile.jumps == 1);

			unit.sync_line(p);
			REQUIRE(profile.drift == 0.11f);
		}

}

TEST_CASE("Rack", "StrangeIO::Component") {
		Rack rack;

		rack.add_unit(unit_uptr(new OmegaUnit("Omega2")));
		auto gwptr = rack.get_unit("Omega2");

		SECTION("Verify mounted units") {
			auto mounted_units = rack.get_units();
			REQUIRE(mounted_units.size() > 0);
		}

		SECTION("Get units") {
			auto wptr = rack.get_unit("foobar");
			REQUIRE(wptr.expired() == true);

			auto gwptr = rack.get_unit("Omega2");
			REQUIRE(gwptr.expired() == false);
		}

		SECTION("Verify unit") {
			auto wptr = rack.get_unit("Omega2");
			auto sptr = wptr.lock();
			REQUIRE(sptr->ulabel() == "Omega2");
		}

		SECTION("Verify unit clearing") {
			rack.clear_units();
			auto wptr = rack.get_unit("Omega2");
			REQUIRE(wptr.expired());
		}

		SECTION("Verify expiration") {
			rack.clear_units();
			REQUIRE(gwptr.expired() == true);
		}

}
