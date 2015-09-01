#include "framework/memory/CacheManager.hpp"

using namespace strangeio::Memory;

CacheManager::CacheManager(int num_blocks) :
m_raw_cache(nullptr), m_num_blocks(num_blocks), m_block_size(0),
m_cache_size(0)
{ }

CacheManager::~CacheManager() {
	if(m_raw_cache != nullptr) delete[] m_raw_cache;
}

const PcmSample* CacheManager::alloc_raw(unsigned int num) {
	if(m_cache_size == 0) return nullptr;

	auto toggle = num;
	PcmSample* ptr = nullptr;

	for(auto& handle : m_handles) {

		if(toggle == 0) break;
		
		if(toggle < num) {
			handle.in_use = true;
			handle.num_blocks = toggle--;
			continue;
		}

		if(!handle.in_use && handle.num_blocks >= num) {
			ptr = handle.ptr;
			handle.in_use = true;
			handle.num_blocks = num;
			toggle--;
		}
	}

	return ptr;
}

void CacheManager::free_raw(const PcmSample* ptr) {

	if(ptr < m_raw_cache || ptr > m_bound) return;
	
	auto index = (ptr - m_raw_cache) / m_block_size;
	auto blocks = m_handles[index].num_blocks;
	auto link = blocks;

	// Get the link index down the line
	if(!m_handles[index+blocks].in_use) {
		link += m_handles[index+blocks].num_blocks;
	}

	// free and link in the blocks
	for(auto i = 0u; i < blocks; i++) {
		auto& handle = m_handles[index+i];
		handle.in_use = false;
		handle.num_blocks = link-i;
	}

	// Relink the blocks up the line
	for(auto i = index; i >= 0; i--) {
		if(m_handles[i].in_use) break;
		m_handles[i].num_blocks = link++;
	}

	/* Cache is now nice and tidy around
	 * the block that has been freed
	 */
}

size_t CacheManager::cache_size() const {
	return m_cache_size;
}

unsigned int CacheManager::num_blocks() const {
	return m_num_blocks;
}

unsigned int CacheManager::block_size() const {
	return m_block_size;
}

void CacheManager::build_cache(unsigned int block_size) {
	m_block_size = block_size;
	
	m_raw_cache = new PcmSample[m_block_size*m_num_blocks];
	m_cache_size = block_size*m_num_blocks;

	auto ptr = m_raw_cache;
	auto nblocks = m_num_blocks;
	
	for(auto i = 0u; i < m_num_blocks; i++) {

		m_handles.push_back(CacheHandle {
				.ptr = (ptr + (i * m_block_size)),
				.in_use = false,
				.num_blocks = nblocks--
		});
	}
	
	m_bound = m_handles[m_num_blocks-1].ptr;
}

#if DEVBUILD
const std::vector<CacheHandle> & CacheManager::get_const_handles() const {
	return m_handles;

}
#endif