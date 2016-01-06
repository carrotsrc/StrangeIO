#include "catch.hpp"
#include "framework/alias.hpp"
#include "framework/log/log.hpp"
#include "framework/routine/debug.hpp"
TEST_CASE( "Test log output", "[strangeio::log]" ) {

	auto& l = strangeio::log::inst();
	
	l << "Hello world, big number: " << 3000 << strangeio::lendl;	
}