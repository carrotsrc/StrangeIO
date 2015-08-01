#ifndef __SOUND_H_1438443855__
#define __SOUND_H_1438443855__
#include "framework/common.h"
namespace RackoonIO {
namespace Helpers {

class SoundRoutines {
public:
	static void deinterleave(const PcmSample* block, unsigned int blockSize, PcmSample* c1, PcmSample* c2);
};

} // helpers
} // RackoonIO
#endif
