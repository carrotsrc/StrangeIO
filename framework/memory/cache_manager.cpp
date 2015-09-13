#include "framework/memory/cache_manager.hpp"

using namespace strangeio::memory;

cache_manager::cache_manager(int num_blocks) 
	: m_raw_cache(nullptr)
	, m_num_blocks(num_blocks)
	, m_block_size(0)
	, m_cache_size(0)
{ }

cache_manager::~cache_manager() {
	if(m_raw_cache != nullptr)
		delete[] m_raw_cache;
}

const PcmSample* cache_manager::alloc_raw(unsigned int num) {
	if(m_cache_size == 0) return nullptr; // not ideal

	std::lock_guard<std::mutex> lg(m_cache_mutex);
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

void cache_manager::free_raw(const PcmSample* ptr) {

	if(ptr < m_raw_cache || ptr > m_bound) return;

	std::lock_guard<std::mutex> lg(m_cache_mutex);

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

size_t cache_manager::cache_size() const {
	return m_cache_size;
}

unsigned int cache_manager::num_blocks() const {
	return m_num_blocks;
}

unsigned int cache_manager::block_size() const {
	return m_block_size;
}

void cache_manager::build_cache(unsigned int block_size) {

	if(m_raw_cache) return;

	m_block_size = block_size;
	
	m_raw_cache = new PcmSample[m_block_size*m_num_blocks];
	m_cache_size = block_size*m_num_blocks;

	auto ptr = m_raw_cache;
	auto nblocks = m_num_blocks;
	
	for(auto i = 0u; i < m_num_blocks; i++) {

		m_handles.push_back(cache_handle {
				.ptr = (ptr + (i * m_block_size)),
				.in_use = false,
				.num_blocks = nblocks--
		});
	}
	
	m_bound = m_handles[m_num_blocks-1].ptr;
}

#if DEVBUILD
const std::vector<cache_handle> & cache_manager::get_const_handles() const {
	return m_handles;

}
#endif