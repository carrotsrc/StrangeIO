#ifndef __SOUND_H_1438443855__
#define __SOUND_H_1438443855__
#include "framework/common.h"
namespace RackoonIO {
namespace Helpers {
namespace SoundRoutines {

	void deinterleave2(const PcmSample* block, PcmSample *out, unsigned int numSamples);
	void interleave2(const PcmSample* block, PcmSample* out, unsigned int numSamples);
}
} // helpers
} // RackoonIO
#endif
