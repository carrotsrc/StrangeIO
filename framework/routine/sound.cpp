#include "framework/routine/sound.hpp"
#include <array>
#include <math.h>

using namespace strangeio::routine;


void strangeio::routine::sound::deinterleave2(const PcmSample* block, PcmSample *out, unsigned int num_frames) {

	auto blockIndex = 0u;
	std::array<PcmSample*, 2> channels;

	for(auto i = 0u; i < 2; i++) {
		channels[i] = out + (num_frames*i);
	}

	for(auto sample = 0u; sample < num_frames; sample++) {
		for(auto channel :  channels) {
			channel[sample] = block[blockIndex++];
		}
	}
}

void strangeio::routine::sound::interleave2(const PcmSample* block, PcmSample* out, unsigned int nun_frames) {
	auto blockIndex = 0u;

	std::array<const PcmSample*, 2> channels;

	for(auto i = 0u; i < 2; i++) {
		channels[i] = block + (nun_frames*i);
	}

	for(auto sample = 0u; sample < nun_frames; sample++) {
		out[blockIndex++] = channels[0][sample];
		out[blockIndex++] = channels[1][sample];
	}
}

void strangeio::routine::sound::sinewave_mono(PcmSample* out, unsigned int fc, unsigned int fs, unsigned int numSamples) {
	constexpr auto pi2 = 2 * M_PI;
	auto omega = pi2*fc;
	for(auto x = 0u; x < numSamples; x++) {
		out[x] = sin( omega * x / fs );
	}

}