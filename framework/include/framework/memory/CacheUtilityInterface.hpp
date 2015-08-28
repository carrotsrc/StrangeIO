#ifndef __CACHEUTILITYINTERFACE_HPP_1440753748__
#define __CACHEUTILITYINTERFACE_HPP_1440753748__
#include "framework/fwcommon.h"
namespace StrangeIO {
namespace Memory {	

class cache_ptr; // Forward declaration
	
class CacheUtilityInterface {
public:
	virtual PcmSample* alloc_raw(int num) = 0;
	virtual void free_raw(PcmSample*) = 0;
}


}
}
#endif
