#include <iostream>
#include <future>

#ifdef __linux__
#include <sched.h>
#endif

#include "catch.hpp"
#include "framework/alias.hpp"

#include "framework/thread/scheduled.hpp"
#include "framework/thread/sched_desc.hpp"


#ifdef __linux__
TEST_CASE( "Assignment operator", "[strangeio::thread],[strangeio::scheduled]" ) {
	siothr::sched_desc sd;
	sd.policy = SCHED_FIFO;
	sd.priority = 99;
	auto sth = siothr::scheduled(sd);
	
	
	REQUIRE(sth.local_policy().policy == sd.policy);
	REQUIRE(sth.local_policy().priority == sd.priority);
}
#endif

#include <chrono>
TEST_CASE( "Start a schedule policy thread", "[strangeio::thread],[strangeio::scheduled]" ) {
	siothr::sched_desc sd;

#ifdef __linux__
	sd.policy = SCHED_FIFO;
	sd.priority = 10;
#endif
	
	std::promise<int> p;
	auto f = p.get_future();
	
	siothr::scheduled sth(sd, [&p]() {
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		p.set_value(101);
	});

	while(!sth.is_active()) continue;
	
	auto desc = sth.assigned_policy();
	f.wait();
	sth.join();
	
	REQUIRE(f.get() == 101);
	
#ifdef __linux__
	REQUIRE(desc.policy == sd.policy);
	REQUIRE(desc.priority == sd.priority);
#endif
	
}
