#ifndef __CACHEMANAGER_HPP_1440753739__
#define __CACHEMANAGER_HPP_1440753739__

#include <vector>
#include "framework/memory/memory.hpp"
#include "framework/memory/cache_utility.hpp"


namespace strangeio {
namespace memory {	


class cache_manager : public cache_utility {
public:
	cache_manager(int num_blocks);
	~cache_manager();
	const PcmSample* alloc_raw(unsigned int num);
	void free_raw(const PcmSample* ptr);

	void build_cache(unsigned int block_size);
	
	size_t cache_size() const;
	unsigned int num_blocks() const;
	unsigned int block_size() const;
	
#if DEVBUILD
	const std::vector<cache_handle>& get_const_handles() const;
#endif

protected:

private:
	PcmSample* m_raw_cache, *m_bound;
	unsigned int m_num_blocks, m_block_size;
	size_t m_cache_size;
	
	std::vector<cache_handle> m_handles;
};

}
}
#endif
