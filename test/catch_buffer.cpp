#include "catch.hpp"
#include "framework/alias.hpp"
#include "framework/memory/cache_manager.hpp"
#include "framework/memory/cache_ptr.hpp"
#include "framework/buffer/circular.hpp"

#include "framework/routine/sound.hpp"

#include <thread>

#define IN_SIZE 518
#define OUT_SIZE 512
#define LOOP_NUM 1000

TEST_CASE( "Test Circular buffer", "[strangeio::buffer],[strangeio::buffer::circular]" ) {

strangeio::buffer::circular cb;
cb.set_size(3*(IN_SIZE+4));

auto iv = 1;
auto ov = 1;

PcmSample *buf_in = new PcmSample[IN_SIZE];
PcmSample *buf_out = new PcmSample[OUT_SIZE];

SECTION("Linear") {
	for( auto i = 0u; i < LOOP_NUM; i++) {
		if(cb.load() < OUT_SIZE) {
			for(auto j = 0; j < IN_SIZE; j++) {
				buf_in[j] = iv++;
			}
			cb.insert(buf_in, IN_SIZE);
		}

		cb.get(buf_out, OUT_SIZE);
		for(auto k = 0u; k < OUT_SIZE; k++) {
			REQUIRE(buf_out[k] == ov++);
		}
	}
}

SECTION("De/Interleaved") {
	PcmSample *buf_work = new PcmSample[IN_SIZE];
	auto ivo = 1;
	for( auto i = 0u; i < LOOP_NUM; i++) {
		
		if(cb.load() < OUT_SIZE) {

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

			cb.insert(buf_work, IN_SIZE);
		}

		auto frames_out = OUT_SIZE/2;
		cb.get(buf_out, OUT_SIZE);
	
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


SECTION("Threaded De/Interleave") {
	std::thread th([&cb, &iv, &ov, &buf_in, &buf_out]() {
		PcmSample *buf_work = new PcmSample[IN_SIZE];
		auto ivo = 1;
		for( auto i = 0u; i < LOOP_NUM; i++) {

			if(cb.load() < OUT_SIZE) {

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

				cb.insert(buf_work, IN_SIZE);
			}

			auto frames_out = OUT_SIZE/2;
			cb.get(buf_out, OUT_SIZE);

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
	});
	
	th.join();
}

}