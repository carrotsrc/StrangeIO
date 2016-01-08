#include "catch.hpp"
#include "framework/alias.hpp"
#include "framework/memory/cache_manager.hpp"
#include "framework/memory/cache_ptr.hpp"
#include "framework/buffer/circular.hpp"

#include "framework/routine/sound.hpp"

#include <thread>

#define IN_SIZE 518
#define OUT_SIZE 512
#define LOOP_NUM 500

TEST_CASE( "Circular Buffer capacity",
"[strangeio::buffer],[strangeio::buffer::circular]\
[strangeio::buffer::circular::capacity][strangeio::buffer::circular::basic]" ) {
	strangeio::buffer::circular<PcmSample> cb;
	
	REQUIRE(cb.capacity() == 0);
	cb.reserve(3);
	REQUIRE(cb.capacity() == 3);
}

TEST_CASE( "Circular Buffer push",
"[strangeio::buffer],[strangeio::buffer::circular]\
[strangeio::buffer::circular::push][strangeio::buffer::circular::basic]" ) {
	strangeio::buffer::circular<PcmSample> cb;
	cb.reserve(3);
	PcmSample s = 0.6f;
	PcmSample m[3] = {0.5f, 0.6f, 0.7f};

	SECTION("rvalue") {
		cb.push_back(0.5f);
		REQUIRE(cb.size() == 1);
	}
	
	SECTION("lvalue copy") {
		
		cb.push_back(s);
		REQUIRE(cb.front() == 0.6f);
		REQUIRE(cb.size() == 1);
		s = 0.7f;
		REQUIRE(cb.front() == 0.6f);
	}
	
	SECTION("lvalue move") {
		cb.push_back(std::move(s));
		REQUIRE(cb.size() == 1);
	}
	
	SECTION("push many") {
		cb.push_many(m, 3);
		REQUIRE(cb.size() == 3);
	}
}

TEST_CASE( "Circular Buffer front",
"[strangeio::buffer],[strangeio::buffer::circular]\
[strangeio::buffer::circular::front][strangeio::buffer::circular::basic]" ) {
	strangeio::buffer::circular<PcmSample> cb;
	cb.reserve(3);
	cb.push_back(0.5f);
	REQUIRE(cb.front() == 0.5);
	REQUIRE(cb.size() == 1);
}

TEST_CASE( "Circular Buffer pop",
"[strangeio::buffer],[strangeio::buffer::circular]\
[strangeio::buffer::circular::pop][strangeio::buffer::circular::basic]" ) {
	strangeio::buffer::circular<PcmSample> cb;
	cb.reserve(3);
	PcmSample s = 0.6f;
	PcmSample m[3] = {0.5f, 0.6f, 0.7f};
	PcmSample out[2] = {0};
	cb.push_many(m, 3);
	
	SECTION("pop_back") {
		cb.pop_front();
		REQUIRE(cb.size() == 2);
	}

	SECTION("pop_many") {
		cb.pop_many(out, 2);
		REQUIRE(cb.size() == 1);
		REQUIRE(out[0] == 0.5f);
		REQUIRE(out[1] == 0.6f);
	}	
	
}

TEST_CASE( "Circular Buffer clear",
"[strangeio::buffer],[strangeio::buffer::circular]\
[strangeio::buffer::circular::clear][strangeio::buffer::circular::basic]" ) {
	strangeio::buffer::circular<PcmSample> cb;
	cb.reserve(3);
	cb.push_back(0.08f);
	cb.clear();
	REQUIRE(cb.size() == 0);
}

TEST_CASE( "Circular Overwrite Full Buffer",
"[strangeio::buffer],[strangeio::buffer::circular]\
[strangeio::buffer::circular::overwrite][strangeio::buffer::circular::basic]" ) {
	strangeio::buffer::circular<PcmSample> cb;
	cb.reserve(3);
	PcmSample a[3] = {0.5f, 0.3f, 0.1f};
	PcmSample b[4] = {0.5f, 0.3f, 0.1f, 0.9f};
	
	cb.push_many(a, 3);
	
	SECTION("Single Overwrite") {
		cb.push_back(0.7f);
		REQUIRE(cb.size() == 3);
		REQUIRE(cb.front() == 0.7f);
	}
	
	SECTION("Many overwrite") {
		cb.push_many(b, 4);
		REQUIRE(cb.size() == 3);
		REQUIRE(cb.front() == 0.9f);		
	}
}

TEST_CASE( "Test Circular buffer", "[strangeio::buffer],[strangeio::buffer::circular]" ) {

strangeio::buffer::circular<PcmSample> cb;
cb.reserve(3*(IN_SIZE+4));

auto iv = 1;
auto ov = 1;

PcmSample *buf_in = new PcmSample[IN_SIZE];
PcmSample *buf_out = new PcmSample[OUT_SIZE];

SECTION("Linear") {
	for( auto i = 0u; i < LOOP_NUM; i++) {
		if(cb.size() < OUT_SIZE) {
			for(auto j = 0; j < IN_SIZE; j++) {
				buf_in[j] = iv++;
			}
			cb.push_many(buf_in, IN_SIZE);
		}

		cb.pop_many(buf_out, OUT_SIZE);
		for(auto k = 0u; k < OUT_SIZE; k++) {
			REQUIRE(buf_out[k] == ov++);
		}
	}
}

SECTION("De/Interleaved") {
	PcmSample *buf_work = new PcmSample[IN_SIZE];
	auto ivo = 1;
	for( auto i = 0u; i < LOOP_NUM; i++) {
		
		if(cb.size() < OUT_SIZE) {

			auto frames = IN_SIZE/2;
			
			for(auto j = 0; j < frames; j++) {
				buf_in[j] = iv;
				buf_in[j+frames] = iv*-1;
				iv++;
			}
			strangeio::routine::sound::interleave2(buf_in, buf_work, frames);

			for(auto j = 0; j < IN_SIZE; j++) {
				REQUIRE(buf_work[j++] == ivo);
				REQUIRE(buf_work[j] == ivo*-1);
				ivo++;
			}

			cb.push_many(buf_work, IN_SIZE);
		}

		auto frames_out = OUT_SIZE/2;
		cb.pop_many(buf_out, OUT_SIZE);
	
		auto ot = ov;
		for(auto j = 0; j < OUT_SIZE; j++) {
			REQUIRE(buf_out[j++] == ot);
			REQUIRE(buf_out[j] == (ot*-1));
			ot++;
		}
		
		strangeio::routine::sound::deinterleave2(buf_out, buf_work, frames_out);

		for(auto j = 0; j < frames_out; j++) {
			REQUIRE(buf_work[j] == ov);
			REQUIRE(buf_work[j+frames_out] == ov*-1);
			ov++;
		}
	}
}

}