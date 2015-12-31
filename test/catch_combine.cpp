#include "catch_combine.hpp"

TEST_CASE("Initial state of Combine", "[strangeio::spec],[strangeio::spec::combine]") {
	Combine unit(std::string("cb1"));
	REQUIRE(unit.unit_profile().state == (int)siocom::line_state::inactive);
	REQUIRE(unit.db_input_active(0) == false);
	REQUIRE(unit.db_input_active(1) == false);
}

TEST_CASE("Test line activation", "[strangeio::spec],[strangeio::spec::combine]") {
	Combine unit(std::string("cb1"));

	siocom::sync_profile profile{0};
	profile.state = (int)siocom::line_state::active;
	auto flags = (siocom::sync_flag)siocom::sync_flags::upstream;

	const_cast<siocom::LinkIn*>(unit.get_input(0))->connected = true;
	const_cast<siocom::LinkIn*>(unit.get_input(1))->connected = true;

	/* State change is inactive -> active
	 * so no global sync is required here
	 */
	SECTION("Single line activation") {
		unit.sync_line(profile, flags, 0);

		REQUIRE(unit.unit_profile().state == (int)siocom::line_state::active);
		REQUIRE(unit.db_input_active(0) == true);
		REQUIRE(unit.db_input_active(1) == false);
	}

	SECTION("Two line activation") {
		unit.sync_line(profile, flags, 0);
		unit.sync_line(profile, flags, 1);

		REQUIRE(unit.unit_profile().state == (int)siocom::line_state::active);
		REQUIRE(unit.db_input_active(0) == true);
		REQUIRE(unit.db_input_active(1) == true);
	}
}

TEST_CASE("Test line deactivation", "[strangeio::spec],[strangeio::spec::combine]") {
	Combine unit(std::string("cb1"));
	const_cast<siocom::LinkIn*>(unit.get_input(0))->connected = true;
	const_cast<siocom::LinkIn*>(unit.get_input(1))->connected = true;

	siocom::sync_profile profile{0};
	profile.state = (int)siocom::line_state::active;


	auto flags = (siocom::sync_flag)siocom::sync_flags::upstream;
	unit.sync_line(profile, flags, 0);
	unit.sync_line(profile, flags, 1);

	/* after an upstream sync, if the global state has changed, the dispatch
	 * unit will trigger a global sync, so all the unit's global profiles
	 * are the same. Here we simulate the global sync.
	 */
	unit.sync_line(profile, (siocom::sync_flag)siocom::sync_flags::glob_sync, 0);
	unit.sync_line(profile, (siocom::sync_flag)siocom::sync_flags::glob_sync, 1);

	
	profile.state = (int)siocom::line_state::inactive;

	SECTION("Single line deactivation") {
		unit.sync_line(profile, flags, 0);

		/* no global sync is necessary here -- not all lines have been
		 * deactivated, meanning the combine unit will not propogate the upstream
		 * inactive profile, which means the dispatch unit won't change state
		 * and so there is no global sync; the rack remains active, while not
		 * all lines and units are active.
		 */

		REQUIRE(unit.global_profile().state == (int)siocom::line_state::active);
		REQUIRE(unit.unit_profile().state == (int)siocom::line_state::active);

		REQUIRE(unit.db_input_active(0) == false);
		REQUIRE(unit.db_input_active(1) == true);
	}

	SECTION("Two line deactivation") {
		unit.sync_line(profile, flags, 0);
		unit.sync_line(profile, flags, 1);

		/* once again, need to globally resync since both lines have been deactated
		 * so the dispatch would receive the upstream inactivity sync, change state 
		 * and trigger a global resync
		 */
		unit.sync_line(profile, (siocom::sync_flag)siocom::sync_flags::glob_sync, 0);
		unit.sync_line(profile, (siocom::sync_flag)siocom::sync_flags::glob_sync, 1);

		REQUIRE(unit.global_profile().state == (int)siocom::line_state::inactive);
		REQUIRE(unit.unit_profile().state == (int)siocom::line_state::inactive);
		
		REQUIRE(unit.db_input_active(0) == false);
		REQUIRE(unit.db_input_active(1) == false);
	}
}