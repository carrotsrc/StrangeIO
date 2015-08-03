#ifndef __SOUND_H_1438443855__
#define __SOUND_H_1438443855__
#include "framework/common.h"
namespace RackoonIO {
namespace Helpers {

class SoundRoutines {
public:
	void inline deinterleave2(const PcmSample* block, PcmSample *out, unsigned int numSamples);
	void inline interleave2(const PcmSample* block, PcmSample* out, unsigned int numSamples);
};

} // helpers
} // RackoonIO
#endif
