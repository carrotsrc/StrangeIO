#ifndef __CACHEMANAGER_HPP_1440753739__
#define __CACHEMANAGER_HPP_1440753739__

#include <vector>
#include "framework/memory/Memory.hpp"
#include "framework/memory/CacheUtilityInterface.hpp"


namespace StrangeIO {
namespace Memory {	


class CacheManager : public CacheUtilityInterface {
public:
	CacheManager(int num_blocks);
	~CacheManager();
	PcmSample* alloc_raw(int num);
	void free_raw(PcmSample* ptr);

	void cache_alloc(int block_size);
	size_t cache_size() const;
	unsigned int num_blocks() const;
	unsigned int block_size() const;

protected:

private:
	PcmSample* m_raw_cache;
	unsigned int m_num_blocks, m_block_size;
	size_t m_cache_size;
	
	std::vector<CacheHandle> m_blocks_ready, m_blocks_inuse;
};

}
}
#endif
