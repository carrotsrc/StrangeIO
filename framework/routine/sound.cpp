#include "framework/routine/sound.hpp"
#include <array>
#include <math.h>

using namespace strangeio::routine;


void strangeio::routine::sound::deinterleave2(const PcmSample* block, PcmSample *out, unsigned int numSamples) {

	auto blockIndex = 0u;
	std::array<PcmSample*, 2> channels;

	for(auto i = 0u; i < 2; i++) {
		channels[i] = out + (numSamples*i);
	}

	for(auto sample = 0u; sample < numSamples; sample++) {
		for(auto channel :  channels) {
			channel[sample] = block[blockIndex++];
		}
	}
}

void strangeio::routine::sound::interleave2(const PcmSample* block, PcmSample* out, unsigned int numSamples) {
	auto blockIndex = 0u;

	std::array<const PcmSample*, 2> channels;

	for(auto i = 0u; i < 2; i++) {
		channels[i] = block + (numSamples*i);
	}

	for(auto sample = 0u; sample < numSamples; sample++) {
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