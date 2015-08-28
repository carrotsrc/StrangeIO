#ifndef __CACHE_PTR_HPP_1440753730__
#define __CACHE_PTR_HPP_1440753730__
#include "framework/fwcommon.hpp"
#include "framework/memory/CacheUtilityInterface.hpp"
namespace StrangeIO {
namespace Memory {

class CachePtr {
public:
	CachePtr(const PcmSample* block, CacheUtilityInterface* cache);
	~CachePtr();

	const PcmSample* release();
	const PcmSample* get() const;
	void reset(const PcmSample* ptr);
	void swap(CachePtr& cptr);
private:
	CacheUtilityInterface* m_cache;
	PcmSample *m_block;
};


}
}
#endif
