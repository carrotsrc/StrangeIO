#ifndef __CacheRw_1440861228__
#define __CacheRw_1440861228__

#include "framework/fwcommon.hpp"
#include "framework/memory/cache_utility.hpp"
#include "framework/memory/cache_ptr.hpp"

namespace strangeio {
namespace memory {	

class cptr_utility {
public:
	cptr_utility(std::string component);
	void set_cache_utility(cache_utility* cache);

protected:
#if CACHE_TRACKING
		virtual cache_ptr cache_alloc(unsigned int num);
#else
		cache_ptr cache_alloc(unsigned int num) const;
#endif
	unsigned int block_size() const;
	void set_utility(cptr_utility& ref) const;
private:
	cache_utility* m_cache;
	std::string m_component_name;
};

} // Memory
} // StrangeIO

#endif

