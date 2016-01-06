#ifndef __CACHEUTILITYINTERFACE_HPP_1440753748__
#define __CACHEUTILITYINTERFACE_HPP_1440753748__
#include "framework/fwcommon.hpp"
namespace strangeio {
namespace memory {	
	
class cache_utility {
public:
	virtual ~cache_utility() { };
#if CACHE_TRACKING
	virtual const PcmSample* alloc_raw(unsigned int num, long* id) = 0;
#else
	virtual const PcmSample* alloc_raw(unsigned int num) = 0;
#endif
	
	virtual void free_raw(const PcmSample*) = 0;
	virtual unsigned int block_size() const = 0;
	virtual void build_cache(unsigned int block_size) = 0;
};


}
}
#endif
