#include <thread>
#include <functional>

#include "catch.hpp"

#include "framework/alias.hpp"
#include "framework/thread/queue.hpp"
#include "framework/thread/ntask_sync.hpp"

TEST_CASE( "Create and destroy handle from ntask_sync", "[strangeio::thread],[strangeio::ntask]" ) {
	siothr::ntask_sync ns;
	REQUIRE(ns.size() == 0);
	{
		auto hndl = ns.make_handle();
		REQUIRE(ns.size() == 1);
	}

	REQUIRE(ns.size() == 0);
}
#include <iostream>
TEST_CASE("Wait for a single task","[strangeio::thread],[strangeio::ntask]" ) {
	siothr::ntask_sync ns;
	siothr::queue queue(3);
	
	queue.start();
	
	while(!queue.is_active()) continue;
	
	ns.start_and_wait([&queue](siothr::ntask_sync_utility& nsu) {
		std::cout << "Running proc" << std::endl;
		auto h = nsu.make_handle();
		queue.add_package([ hndl{ std::move(h) } ] () mutable {
			std::cout << "Called lambda" << std::endl;
			WARN(hndl.get_id());
			REQUIRE(hndl.get_id() == 1);
			std::this_thread::sleep_for(std::chrono::milliseconds(500));	
		});
	});
	
	std::cout << "RUnning task" << std::endl;
	ns.go();
	std::cout << "Waiting for end of queue" << std::endl;
	queue.stop();
	while(queue.is_active()) continue;
}