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

		SECTION("Verify unit configurations") {
			unit.set_configuration("test_key", "1001");
			REQUIRE(unit.config_test() == 1001);
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

		rack.add_mainline("ac1");
		auto omega = new OmegaUnit("Omega2");
		auto epsilon = new EpsilonUnit("Epsilon1");
		omega->delayed_constructor();
		epsilon->delayed_constructor();

		rack.add_unit(unit_uptr(omega));
		rack.add_unit(unit_uptr(epsilon));


		auto gwptr = rack.get_unit("Omega2");

		SECTION("Verify mounted units") {
			auto mounted_units = rack.get_units();
			REQUIRE(mounted_units.size() > 0);
		}

		SECTION("Get units") {
			auto wptr = rack.get_unit("foobar");
			REQUIRE(wptr.expired() == true);

			wptr = rack.get_unit("Omega2");
			REQUIRE(gwptr.expired() == false);

			wptr = rack.get_unit("Epsilon1");
			REQUIRE(gwptr.expired() == false);
		}

		SECTION("Verify unit") {
			auto wptr = rack.get_unit("Omega2");
			auto sptr = wptr.lock();
			REQUIRE(sptr->ulabel() == "Omega2");

			wptr = rack.get_unit("Epsilon1");
			sptr = wptr.lock();
			REQUIRE(sptr->ulabel() == "Epsilon1");
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

		SECTION("Verify faulty connections") {
			REQUIRE(rack.connect_mainline("ac2", "Omega2") == false);
			REQUIRE(rack.connect_mainline("ac1", "Omega3") == false);
			REQUIRE(rack.connect_units("Omega3", "audio", "Epsilon1", "audio_in") == false);
			REQUIRE(rack.connect_units("Omega2", "foobar", "Epsilon1", "audio_in") == false);
			REQUIRE(rack.connect_units("Omega2", "audio", "Epsilon2", "audio_in") == false);
			REQUIRE(rack.connect_units("Omega2", "audio", "Epsilon1", "foobar") == false);

		}

		SECTION("Verify empty cycle") {
			REQUIRE(rack.cycle(CycleType::Warmup) == CycleState::Empty);
		}

		SECTION("Verify connect") {
			REQUIRE(rack.connect_mainline("ac1", "Omega2") == true);
			REQUIRE(rack.connect_units("Omega2", "audio", "Epsilon1", "audio_in") == true);
		}

}

TEST_CASE("Cycle Cascades", "StrangeIO::Component") {
		Rack rack;

		rack.add_mainline("ac1");
		auto omega = new OmegaUnit("Omega2");
		auto epsilon = new EpsilonUnit("Epsilon1");
		omega->delayed_constructor();
		epsilon->delayed_constructor();

		rack.add_unit(unit_uptr(omega));
		rack.add_unit(unit_uptr(epsilon));

		rack.connect_mainline("ac1", "Omega2");
		rack.connect_units("Omega2", "audio", "Epsilon1", "audio_in");

		SECTION("Verify Warmup cycle") {
			REQUIRE(rack.cycle(CycleType::Warmup) != CycleState::Empty);
			auto wptr = rack.get_unit("Omega2");
			auto sptr = wptr.lock();
			auto omega = std::static_pointer_cast<OmegaUnit>(sptr);
			REQUIRE(omega->init_count() == 1);
		}

		SECTION("Verify Sync cycle") {
			rack.cycle(CycleType::Warmup);
			REQUIRE(rack.rack_profile().sync_duration == ProfileDuration::zero());
			rack.sync((SyncFlag)SyncFlags::SyncDuration);
			REQUIRE(rack.rack_profile().sync_duration != ProfileDuration::zero());
		}

		SECTION("Verify Cycle cascade") {
			rack.cycle(CycleType::Warmup);

			REQUIRE(omega->init_count() == 1);
			REQUIRE(epsilon->init_count() == 1);
		}

		SECTION("Verify Sync cascade") {
			rack.cycle(CycleType::Warmup);
			Profile profile{0};
			
			REQUIRE(rack.profile_line(profile, "ac1") == true);

			REQUIRE(profile.jumps == 2);
			REQUIRE(profile.fs == 44100);
			REQUIRE(profile.drift != omega->unit_profile().drift);
			REQUIRE(profile.drift != epsilon->unit_profile().drift);

			auto drift_actual = omega->unit_profile().drift + (omega->unit_profile().drift * epsilon->unit_profile().drift);
			REQUIRE(profile.drift == drift_actual);
		}

		SECTION("Verify Sync cascade") {
			rack.cycle(CycleType::Warmup);
			Profile profile{0};
			
			REQUIRE(rack.profile_line(profile, "ac1") == true);

			REQUIRE(profile.jumps == 2);
			REQUIRE(profile.fs == 44100);
			REQUIRE(profile.drift != omega->unit_profile().drift);
			REQUIRE(profile.drift != epsilon->unit_profile().drift);

			auto drift_actual = omega->unit_profile().drift + (omega->unit_profile().drift * epsilon->unit_profile().drift);
			REQUIRE(profile.drift == drift_actual);
		}

		SECTION("Verify Feed cascade") {
			rack.cycle(CycleType::Warmup);
			rack.cycle(CycleType::Ac);

			REQUIRE(omega->feed_count() == 0);
			REQUIRE(epsilon->feed_count() == 1);
			REQUIRE(epsilon->feed_check() == 0.9f);
		}
}

TEST_CASE("Partial Cycles", "StrangeIO::Component") {
		Rack rack;

		rack.add_mainline("ac1");
		rack.add_mainline("ac2");

		auto omega_a = new OmegaUnit("Omega_A");
		auto omega_b = new OmegaUnit("Omega_B");
		auto delta = new DeltaUnit("Delta");
		auto epsilon = new EpsilonUnit("Epsilon");

		omega_a->delayed_constructor();
		omega_b->delayed_constructor();
		epsilon->delayed_constructor();

		rack.add_unit(unit_uptr(omega_a));
		rack.add_unit(unit_uptr(omega_b));
		rack.add_unit(unit_uptr(epsilon));
		rack.add_unit(unit_uptr(delta));



		rack.connect_mainline("ac1", "Omega_A");
		rack.connect_units("Omega_A", "audio", "Delta", "channel_a");
		rack.connect_units("Delta", "audio", "Epsilon", "audio_in");

		SECTION("Verify single active channel step") {
			rack.cycle(CycleType::Warmup);
			REQUIRE(omega_a->init_count() == 1);
			REQUIRE(delta->init_count() == 1);
			REQUIRE(epsilon->init_count() == 1);

			rack.cycle();
			REQUIRE(delta->feed_count() == 1);
			REQUIRE(epsilon->feed_count() == 1);
		}

		SECTION("Verify two active channel partial cycle") {
			rack.connect_mainline("ac2", "Omega_B");
			REQUIRE(rack.connect_units("Omega_B", "audio", "Delta", "channel_b") == true);
			rack.cycle(CycleType::Warmup);

			REQUIRE(omega_b->init_count() == 1);

			rack.cycle();
			REQUIRE(delta->partial_count() == 1);
			REQUIRE(delta->feed_count() == 2);
			REQUIRE(epsilon->feed_count() == 1);
		}
}
