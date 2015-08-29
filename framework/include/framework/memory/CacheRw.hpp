#ifndef __CacheRw_1440861228__
#define __CacheRw_1440861228__

#include "framework/fwcommon.hpp"
#include "framework/memory/CacheUtilityInterface.hpp"
#include "framework/memory/CachePtr.hpp"

namespace StrangeIO {
namespace Memory {	

class CacheRw {
public:
	CacheRw();
	void set_cache_utility(CacheUtilityInterface* cache);

protected:
	CachePtr cache_alloc(unsigned int num);
private:
	CacheUtilityInterface* m_cache;
};

} // Memory
} // StrangeIO

#endif

