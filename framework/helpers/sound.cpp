#include "sound.h"

using namespace RackoonIO::Helpers;

void SoundRoutines::deinterleave2(const PcmSample* block, PcmSample *out, unsigned int numSamples) {

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

void SoundRoutines::interleave2(const PcmSample* block, PcmSample* out, unsigned int numSamples) {
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
