#ifndef __CACHEMANAGER_HPP_1440753739__
#define __CACHEMANAGER_HPP_1440753739__
#include "framework/memory/CacheUtilityInterface.hpp"
class CacheManager : public CacheUtilityInterface {
public:
	CacheManager(int num_blocks);
	PcmSample* alloc_raw(int num);
	void free_raw(PcmSample* ptr);

	bool cache_alloc(size_t cache_size, int block_size);
	size_t cache_size() const;
	int num_blocks() const;
	int block_size() const;

protected:

private:
	PcmSample* m_cache;
	int m_num_blocks, m_block_size;
	size_t m_cache_size;
}
#endif
