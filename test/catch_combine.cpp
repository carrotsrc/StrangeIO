#include "catch_combine.hpp"

TEST_CASE("Initial state of Combine", "[strangeio::spec],[strangeio::spec::combine]") {
	Combine unit(std::string("cb1"));
	REQUIRE(unit.db_input_active(0) == false);
	REQUIRE(unit.db_input_active(1) == false);
}