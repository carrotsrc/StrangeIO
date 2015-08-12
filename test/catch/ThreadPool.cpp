#define CATCH_CONFIG_MAIN

#include <future>
#include "catch.hpp"
#include "framework/threads/ThreadPool.h"
//#include "framework/threads/ThreadPool.h"

using namespace StrangeIO;

TEST_CASE( "Start and stop thread pool", "[ThreadPoolStart]" ) {
	std::condition_variable cv;

	ThreadPool pool(1);
	REQUIRE( pool.getSize() == 1 );

	pool.setSize(2);
	REQUIRE( pool.getSize() == 2 );

	pool.init(&cv);
	auto numRunning = 0u;
	for(auto i = 0u; i < pool.getSize(); i++) {
		auto worker = pool[i];
		if(worker->isRunning()) {
			numRunning++;
		}
	}
	REQUIRE( numRunning == pool.getSize() );

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

TEST_CASE( "Start and stop RackQueue", "[RackQueueStartStop]" ) {
	std::condition_variable cv;
	std::promise<int> p;
	std::future<int> f = p.get_future();
	ThreadPool pool(2);
	pool.init(&cv);

	auto worker = pool[0];
	worker->assignPackage(std::unique_ptr<WorkerPackage>(new WorkerPackage(
					[&p](){ 
						p.set_value(1001); 
					}
					)));

	REQUIRE( worker->isLoaded() );

	worker->notify();
	f.wait();

	REQUIRE( f.get() == 1001 );

}

