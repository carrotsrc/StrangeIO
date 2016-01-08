#include "framework/routine/sound.hpp"
#include "framework/alias.hpp"
#include <array>
#include <math.h>

using namespace strangeio::routine;


void strangeio::routine::sound::deinterleave2(const PcmSample* in, PcmSample *out, unsigned int num_frames) {
	auto blockIndex = 0u;
	std::array<PcmSample*, 2> channels;

	for(auto i = 0u; i < 2; i++) {
		channels[i] = out + (num_frames*i);
	}

	for(auto sample = 0u; sample < num_frames; sample++) {
		for(auto channel :  channels) {
			channel[sample] = in[blockIndex++];
		}
	}
}

void strangeio::routine::sound::deinterleave2(siomem::cache_ptr in, PcmSample *out, unsigned int num_frames) {
	if(!in) throw strangeio::cache_nullptr("sound::routine::interleave");
#if CACHE_TRACKING
	in.set_owner({-1,siocom::ctype::unspec, "snd::routine","deinterleave2"});
#endif
	strangeio::routine::sound::deinterleave2(*in, out, num_frames);
}

void strangeio::routine::sound::interleave2(const PcmSample* in, PcmSample* out, unsigned int nun_frames) {
	if(in == nullptr || out == nullptr) throw strangeio::cache_nullptr("sound::routine::interleave");
	auto blockIndex = 0u;

	std::array<const PcmSample*, 2> channels;

	for(auto i = 0u; i < 2; i++) {
		channels[i] = in + (nun_frames*i);
	}

	for(auto sample = 0u; sample < nun_frames; sample++) {
		out[blockIndex++] = channels[0][sample];
		out[blockIndex++] = channels[1][sample];
	}
}

void strangeio::routine::sound::interleave2(siomem::cache_ptr in, PcmSample *out, unsigned int num_frames) {
	if(!in) throw strangeio::cache_nullptr("sound::routine::interleave");
#if CACHE_TRACKING
	in.set_owner({-1,siocom::ctype::unspec, "snd::routine","interleave2"});
#endif
	strangeio::routine::sound::interleave2(*in, out, num_frames);
}

void strangeio::routine::sound::sinewave_mono(PcmSample* out, unsigned int fc, unsigned int fs, unsigned int numSamples) {
	constexpr auto pi2 = 2 * M_PI;
	auto omega = pi2*fc;
	for(auto x = 0u; x < numSamples; x++) {
		out[x] = sin( omega * x / fs );
	}

}