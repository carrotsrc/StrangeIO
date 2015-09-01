#ifndef __CACHE_PTR_HPP_1440753730__
#define __CACHE_PTR_HPP_1440753730__
#include "framework/fwcommon.hpp"
#include "framework/memory/CacheUtilityInterface.hpp"
namespace strangeio {
namespace Memory {

class CachePtr {
public:
	CachePtr();
	CachePtr(const PcmSample* block, unsigned int num_blocks, CacheUtilityInterface* cache);
	CachePtr(CachePtr& that);
	CachePtr(CachePtr&& that);
	~CachePtr();

	unsigned int num_blocks() const;
	unsigned int block_size() const;

	const PcmSample* release();
	const PcmSample* get() const;
	void reset(const PcmSample* ptr, unsigned int num_blocks = 1);
	void swap(CachePtr& cptr);

	const PcmSample* operator *() const;
	PcmSample& operator [](int index);
	CachePtr& operator =(CachePtr& that);
	operator bool() const;

private:
	CacheUtilityInterface* m_cache;
	PcmSample* m_block;
	unsigned int m_num_blocks;
};


}
}
#endif
