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

#include <future>
#include "catch.hpp"
#include "framework/threads/ThreadPool.h"
#include "framework/threads/RackQueue.h"
//#include "framework/threads/ThreadPool.h"
#if !DEVBUILD
	#error The testing suite requires DEVBUILD to be enabled
#endif
using namespace strangeio;

TEST_CASE( "Start and stop thread pool", "[ThreadPoolStart]" ) {
	std::condition_variable cv;

	thread_pool pool(1);
	REQUIRE( pool.getSize() == 1 );

	pool.setSize(2);
	REQUIRE( pool.getSize() == 2 );

	REQUIRE( pool[4] == nullptr);

	pool.init(&cv);
	auto numRunning = 0u;
	for(auto i = 0u; i < pool.getSize(); i++) {
		auto worker = pool[i];
		if(worker->isRunning()) {
			numRunning++;
		}
	}
	REQUIRE( numRunning == pool.getSize() );

	REQUIRE( pool[4] == nullptr);
	REQUIRE( pool[0] != nullptr);

	pool.stop();

	auto numStopped = 0u;
	for(auto i = 0u; i < pool.getSize(); i++) {
		auto worker = pool[i];
		if(!worker->isActive()) {
			numStopped++;
		}
	}
	REQUIRE( numStopped == pool.getSize() );
}

TEST_CASE( "Assign task to thread pool", "[ThreadPoolAssign]" ) {
	std::condition_variable cv;
	std::promise<int> p;
	std::future<int> f = p.get_future();
	thread_pool pool(2);
	pool.init(&cv);

	auto worker = pool[0];
	worker->assignPackage(std::unique_ptr<pkg>(new pkg(
					[&p](){ 
						p.set_value(303); 
					}
					)));

	REQUIRE( worker->isLoaded() );

	worker->notify();
	f.wait_for(std::chrono::milliseconds(20));

	REQUIRE( f.get() == 303 );
}

TEST_CASE( "Start rack queue and stop", "[RackQueueStartStop]" ) {
	RackQueue queue(0);
	REQUIRE( queue.getSize() == 0 );

	queue.setSize(2);
	REQUIRE( queue.getSize() == 2 );
	
	REQUIRE( queue[10] == nullptr );
	queue.init();

	REQUIRE( queue.isRunning() == true );
	REQUIRE( queue[0] != nullptr );
	REQUIRE( queue[0]->isRunning() == true );
	REQUIRE( queue[10] == nullptr );

	// Wait for the queue to fully start up
	std::this_thread::sleep_for(std::chrono::milliseconds(20));
	REQUIRE( queue.isActive() == true );

	queue.stop();
	REQUIRE( queue[0] != nullptr );
	REQUIRE( queue[0]->isActive() == false );
	REQUIRE( queue.isRunning() == false );
	REQUIRE( queue.isActive() == false );
}

TEST_CASE( "Assign task to RackQueue", "[RackQueueAssign]" ) {
	std::promise<int> p;
	std::future<int> f = p.get_future();
	RackQueue queue(2);
	queue.init();

	REQUIRE( queue.getSize() == 2 );

	// Wait for it to full start up
	std::this_thread::sleep_for(std::chrono::milliseconds(20));
	REQUIRE( queue.isRunning() == true );
	REQUIRE( queue[1]->isRunning() == true );

	queue.addPackage([&p]() { 
						p.set_value(808); 
					});

	f.wait();


	REQUIRE( f.get() == 808 );
}
