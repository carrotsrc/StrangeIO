#include "sound.h"

using namespace RackoonIO::Helpers;

void SoundRoutines::deinterleave(const PcmSample* block, unsigned int blockSize, PcmSample* c1, PcmSample* c2) {
	auto j = 0u;
	for(auto i = 0u; i < blockSize; i++) {

		if(i%2 == 0) {
			c1[j] = block[i];
		} else {
			c2[j++] = block[i];
		}
	}
}

