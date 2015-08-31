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
#include "framework/memory/CacheManager.hpp"
using namespace StrangeIO::Memory;
TEST_CASE("CacheManager", "[StrangeIO::Memory]") {
	CacheManager cache(32);

	SECTION("Verify Initial State") {
		REQUIRE(cache.num_blocks() == 32);
	}

	SECTION("Verify uninitialised allocation") {
		REQUIRE(cache.alloc_raw(1) == nullptr);
	}

	SECTION("Verify cache sizes") {
		cache.build_cache(512);
		REQUIRE(cache.block_size() == 512);
		REQUIRE(cache.cache_size() == 512*32);
		auto handles = cache.get_const_handles();
		REQUIRE(handles.size() == 32);
		REQUIRE(handles[0].num_blocks == 32);
		REQUIRE(handles[handles.size()-1].num_blocks == 1);
	}

	SECTION("Verify single allocation") {
		cache.build_cache(512);
		auto ptr = cache.alloc_raw(1);
		REQUIRE(ptr != nullptr);
		auto& handles = cache.get_const_handles();
		REQUIRE(handles[0].ptr == ptr);
		REQUIRE(handles[0].in_use == true);
		REQUIRE(handles[0].num_blocks == 1);
	}

	SECTION("Verify multiple 1 block allocations") {
		cache.build_cache(512);
		auto& handles = cache.get_const_handles();
		auto ptr = cache.alloc_raw(1);

		REQUIRE(ptr != nullptr);

		REQUIRE(handles[0].ptr == ptr);
		REQUIRE(handles[0].in_use == true);
		REQUIRE(handles[0].num_blocks == 1);
		
		auto ptr2 = cache.alloc_raw(1);
		REQUIRE(ptr2 != nullptr);
		REQUIRE(handles[1].ptr == ptr2);
		REQUIRE(handles[1].in_use == true);
		REQUIRE(handles[1].num_blocks == 1);
	}

	SECTION("Verify single multi-block allocation") {
		cache.build_cache(512);
		auto& handles = cache.get_const_handles();
		auto ptr = cache.alloc_raw(5);
		
		REQUIRE(ptr != nullptr);
		REQUIRE(handles[0].ptr == ptr);
		REQUIRE(handles[0].in_use == true);
		REQUIRE(handles[0].num_blocks == 5);

		REQUIRE(handles[4].in_use == true);
		REQUIRE(handles[4].num_blocks == 1);

		REQUIRE(handles[5].in_use == false);
	}

	SECTION("Verify multiple multi-block allocations") {
		cache.build_cache(512);
		auto& handles = cache.get_const_handles();
		auto ptr = cache.alloc_raw(5);
		auto ptr2 = cache.alloc_raw(3);

		REQUIRE(handles[5].ptr == ptr2);
		REQUIRE(handles[5].in_use == true);
		REQUIRE(handles[5].num_blocks == 3);
		REQUIRE(handles[7].num_blocks == 1);
		REQUIRE(handles[8].in_use == false);
	}

	SECTION("Verify single block free") {
		cache.build_cache(512);
		auto& handles = cache.get_const_handles();
		auto ptr = cache.alloc_raw(1);
		cache.free_raw(ptr);

		REQUIRE(handles[0].ptr == ptr);
		REQUIRE(handles[0].in_use == false);
		REQUIRE(handles[0].num_blocks == 32);
	}

	SECTION("Verify multiple single block free") {
		cache.build_cache(512);
		auto& handles = cache.get_const_handles();
		auto ptr = cache.alloc_raw(1);
		auto ptr2 = cache.alloc_raw(1);
		cache.free_raw(ptr);

		REQUIRE(handles[0].ptr == ptr);
		REQUIRE(handles[0].in_use == false);
		REQUIRE(handles[0].num_blocks == 1);

		REQUIRE(handles[1].ptr == ptr2);
		REQUIRE(handles[1].in_use == true);
		REQUIRE(handles[1].num_blocks == 1);

		cache.free_raw(ptr2);
		REQUIRE(handles[1].ptr == ptr2);
		REQUIRE(handles[1].in_use == false);

		// Check downward relink
		REQUIRE(handles[1].num_blocks == 31);

		// Check upward relink
		REQUIRE(handles[0].num_blocks == 32);
	}
	
	SECTION("Verify single multi-block free") {
		cache.build_cache(512);
		auto& handles = cache.get_const_handles();
		auto ptr = cache.alloc_raw(5);
		cache.free_raw(ptr);

		REQUIRE(handles[0].ptr == ptr);
		REQUIRE(handles[0].num_blocks == 32);
		REQUIRE(handles[4].in_use == false);
		REQUIRE(handles[4].num_blocks == 28);
	}

	SECTION("Verify multiple multi-block free") {
		cache.build_cache(512);
		auto& handles = cache.get_const_handles();
		auto ptr = cache.alloc_raw(5);
		auto ptr2 = cache.alloc_raw(3);
		cache.free_raw(ptr);

		REQUIRE(handles[0].num_blocks == 5);
		REQUIRE(handles[0].in_use == false);
		
		cache.free_raw(ptr2);
		REQUIRE(handles[5].ptr == ptr2);

		// Check upward relink
		REQUIRE(handles[5].num_blocks == 27);
		REQUIRE(handles[5].in_use == false);

		// Check downward relink
		REQUIRE(handles[0].num_blocks == 32);
	}
}

#include "framework/memory/CachePtr.hpp"

TEST_CASE("CachePtr", "[StrangeIO::Memory]") {
	CacheManager cache(32);
	cache.build_cache(512);
	auto& handles = cache.get_const_handles();

	SECTION("Test creation and destruction") {
		auto cptr = new CachePtr(cache.alloc_raw(3), 3, &cache);
		REQUIRE(cptr->get() == handles[0].ptr);
		REQUIRE(cptr->block_size() == 512);
		REQUIRE(cptr->num_blocks() == 3);
		REQUIRE(handles[0].in_use == true);

		delete cptr;
		REQUIRE(handles[0].in_use == false);
	}
	
	SECTION("Test empty state") {
		auto cptr = CachePtr();
		REQUIRE(cptr.get() == nullptr);
		REQUIRE(cptr.num_blocks() == 0);
	}

	SECTION("Test boolean operator overload") {
		auto cptr = CachePtr();
		REQUIRE(cptr.get() == nullptr);
		REQUIRE(cptr == false);
		auto cptr2 = CachePtr(cache.alloc_raw(1), 1, &cache);
		REQUIRE(cptr2.get() != nullptr);
		REQUIRE(cptr2 ==  true);
	}

	SECTION("Test dereference") {
		CachePtr cptr(cache.alloc_raw(3), 3, &cache);
		REQUIRE(*cptr == handles[0].ptr);
	}

	SECTION("Test array access") {
		CachePtr cptr(cache.alloc_raw(3), 3, &cache);
		cptr[6] = 123.321f;
		REQUIRE(cptr[6] == 123.321f);
	}

	SECTION("Test release") {
		auto cptr = new CachePtr(cache.alloc_raw(3), 3, &cache);
		auto ptr = cptr->release();
		REQUIRE(cptr->num_blocks() == 0);
		REQUIRE(cptr->get() == nullptr);
		REQUIRE(handles[0].in_use == true);
		delete cptr;
		cache.free_raw(ptr);
	}

	SECTION("Test swap and release") {
		auto cptr = new CachePtr(cache.alloc_raw(3), 3, &cache);
		auto cptr2 = new CachePtr(cache.alloc_raw(2), 1, &cache);
		
		REQUIRE(cptr2->get() == handles[3].ptr);
		cptr->swap(*(cptr2));
		REQUIRE(cptr2->get() == handles[0].ptr);
		REQUIRE(cptr->get() == handles[3].ptr);
		
		auto ptr = cptr->release();
		REQUIRE(cptr->num_blocks() == 0);
		REQUIRE(cptr->get() == nullptr);
		REQUIRE(handles[3].in_use == true);
		
		delete cptr2;
		delete cptr;
		cache.free_raw(ptr);
	}
	
	SECTION("Test reset") {
		auto cptr = new CachePtr(cache.alloc_raw(3), 3, &cache);
		cptr->reset(cache.alloc_raw(1), 1);

		REQUIRE(handles[0].in_use == false);
		REQUIRE(handles[3].in_use == true);
		REQUIRE(cptr->get() == handles[3].ptr);
	}
	
	SECTION("Verify scope deletion") {
		{
			CachePtr cptr(cache.alloc_raw(3), 3, &cache);
			REQUIRE(handles[0].in_use == true);
		}
		REQUIRE(handles[0].in_use == false);
	}

	SECTION("Verify copy constructor") {
		CachePtr cptr(cache.alloc_raw(2), 2, &cache);
		CachePtr cpy = cptr;
		REQUIRE(cpy.get() == handles[0].ptr);
		REQUIRE(cpy.num_blocks() == 2);
		REQUIRE(cptr.get() == nullptr);
		REQUIRE(cptr.num_blocks() == 0);
	}
	
	SECTION("Verify move constructor") {
		auto cptr = CachePtr(cache.alloc_raw(4), 4, &cache);
		REQUIRE(cptr.get() == handles[0].ptr);
		REQUIRE(cptr.num_blocks() == 4);
	}
	
	SECTION("Verify assignment operator") {
		auto cptr = CachePtr(cache.alloc_raw(4), 4, &cache);
		auto cptr2 = CachePtr();
		REQUIRE(cptr.get() == handles[0].ptr);
		REQUIRE(cptr2.get() == nullptr);
		cptr2 = cptr;
		REQUIRE(cptr.get() == nullptr);
		REQUIRE(cptr2.get() == handles[0].ptr);
	}
}

#include "framework/midi/DriverUtilityInterface.hpp"
using namespace StrangeIO::Midi;

TEST_CASE("DriverUtilityInterface", "[StrangeIO::Midi]") {
	auto midi_interface = DriverUtilityInterface();

	SECTION("Get input handle") {
		auto handle = midi_interface.open_input_port("TestName","hw:1,0,0");
		CHECK(handle.get() != nullptr);
		if(handle.get() != nullptr) {
			midi_interface.close_input_port(std::move(handle));
		}
	}
}

#include <future>
#include "framework/midi/DriverUtilityInterface.hpp"
#include "framework/midi/MidiDevice.hpp"

TEST_CASE("MidiDevice", "[StrangeIO::Midi]") {
	auto midi_interface = DriverUtilityInterface();
	auto device = MidiDevice("hw:1,0,0", "TestName", &midi_interface);
	
	SECTION("Test Midi device init") {
		auto state = device.init();
		CHECK(state == true);
		if(state) {
			device.close_handle();
		}
	}

	/* Todo:
	 * 
	 * These tests need to be run when the midi
	 * device thread can end gracefully
	 * 
	 *	SECTION("Start and Stop Midi device") {
	 *		if(device.init() == true) {
	 *			device.start();
	 *		} else {
	 *			WARN("No midi device present");
	 *		}
	 * 	}
	 */
	
	SECTION("Test sync Midi input") {
		if(device.init() == true) {
			auto f = 303u;
			auto controller_id = 50;

			device.add_binding(controller_id, [&f](MidiCode) {
				f = 808u;
			});

			WARN("Waiting for midi input on controller " << controller_id);
			device.test_cycle();
			REQUIRE(f == 808u);
			device.close_handle();
		} else {
			WARN("No midi device present");
		}
	}

}

TEST_CASE("MidiHandler", "[StrangeIO::Midi]") {
	auto midi_interface = DriverUtilityInterface();
}

TEST_CASE( "Unit", "[StrangeIO::Component]" ) {

		AlphaUnit unit("Alpha1");
		LinkIn lin {
			.label = "nowhere",
			.id = 0xf,
			.unit = nullptr
		};

		SECTION("Verify unit attributes") {
			REQUIRE(unit.utype() == UnitType::Mainliner);
			REQUIRE(unit.umodel() == "Alpha");
			REQUIRE(unit.ulabel() == "Alpha1");
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

TEST_CASE("Unit Midi Binding", "[StrangeIO::Component],[StrangeIO::Midi]") {
	auto mu = MuUnit("mu");
	auto interface = DriverUtilityInterface();
	auto device = MidiDevice("hw:1,0,0", "TestController", &interface);
	auto controller_id = 50u;
	
	SECTION("Verify registered handlers") {
		auto handlers = mu.midi_handlers();
		REQUIRE(handlers.size() > 0);
		REQUIRE(handlers.find("mu_bind") != handlers.end());
		handlers["mu_bind"](MidiCode{0});
		REQUIRE(mu.midi_count() > 0);
	}
	SECTION("Verify binding") {
		if(device.init()) {
			auto handlers = mu.midi_handlers();
			device.add_binding(controller_id, handlers["mu_bind"]);
			WARN("Waiting for Midi input from controller " << controller_id);
			device.test_cycle();
			device.close_handle();
			REQUIRE(mu.midi_count() > 0);
		} else {
			WARN("Midi device not found");
		}
	}
}

TEST_CASE("Rack", "[StrangeIO::Component]") {
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

TEST_CASE("Cycle Cascades", "[StrangeIO::Component]") {
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

TEST_CASE("Partial Cycles", "[StrangeIO::Component]") {
		Component::Rack rack;

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

TEST_CASE("Cache management in cycle", "[StrangeIO::Component]") {
		Component::Rack rack;
		Memory::CacheManager cache(32);
		
		cache.build_cache(512);
		const auto& handles = cache.get_const_handles();

		rack.set_cache_utility(&cache);
		rack.add_mainline("ac1");
		
		auto phi = new PhiUnit("phi");
		auto tau = new TauUnit("tau");

		rack.add_unit(unit_uptr(phi));
		rack.add_unit(unit_uptr(tau));

		rack.connect_mainline("ac1", "phi");
		rack.connect_units("phi", "audio", "tau", "audio_in");
		rack.cycle(CycleType::Warmup);
		rack.cycle(CycleType::Sync);

		SECTION("Verify links") {
			REQUIRE(phi->init_count() == 1);
			REQUIRE(tau->init_count() == 1);
		}

		SECTION("Verify cache alloc and free") {
			rack.cycle(CycleType::Ac);
			REQUIRE(tau->feed_count() == 1);
			REQUIRE(tau->block_count() == 5);
			REQUIRE(tau->ptr() == handles[0].ptr);
			REQUIRE(handles[0].in_use == false);
			REQUIRE(handles[0].num_blocks == cache.num_blocks());
		}
}