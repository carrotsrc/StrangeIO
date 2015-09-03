#ifndef __SOUND_H_1438443855__
#define __SOUND_H_1438443855__
#include "framework/fwcommon.hpp"
namespace strangeio {
namespace routine {
namespace sound {

	void deinterleave2(const PcmSample* block, PcmSample *out, unsigned int numSamples);
	void interleave2(const PcmSample* block, PcmSample* out, unsigned int numSamples);
}
} // helpers
} // StrangeIO
#endif
