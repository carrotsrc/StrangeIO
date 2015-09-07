#include "framework/routine/sound.hpp"
#include <array>
using namespace strangeio::routine;

void strangeio::routine::sound::deinterleave2(PcmSample* block, unsigned int period_size) {
	PcmSample ax, bx;

	auto end = (period_size*2)-1;
	auto c1 = 1u, c2 = end-1;

	auto inner = period_size-1;
	auto outer = period_size/2;

	for(auto i = c1; i <= outer; i++) {
		ax = block[c1];
		bx = block[c2];

		for(auto j = c1; j < inner; j++) {
			block[j] = block[j+1];
			block[end-j] = block[end-(j+1)];
		}
		block[period_size-i] = bx;
		block[end - inner] = ax;
		++c1;
		--c2;
		--inner;
	}
}

void strangeio::routine::sound::interleave2(PcmSample* block, unsigned int period_size) {
	PcmSample ax;
	auto c1 = 1u;
	auto c2 = period_size;
	auto range = period_size*2-1;

	while(c1 < range) {
		ax = block[c2];
		for(auto j = c2; j > c1; j--) {
			block[j] = block[j-1];
		}
		block[c1] = ax;
		c2++;
		c1 += 2;
	}
}
