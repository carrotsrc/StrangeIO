#include "framework/memory/CacheManager.hpp"

using namespace StrangeIO::Memory;

CacheManager::CacheManager(int num_blocks) :
m_raw_cache(nullptr), m_num_blocks(num_blocks), m_block_size(0),
m_cache_size(0)
{ }

CacheManager::~CacheManager() {
	delete m_raw_cache;
}

#include <iostream>
PcmSample* CacheManager::alloc_raw(unsigned int num) {
	if(m_cache_size == 0) return nullptr;

	auto toggle = num;
	PcmSample* ptr = nullptr;

	for(auto& handle : m_handles) {
		if(toggle < num) {
			handle.in_use = true;
			handle.num_blocks = toggle;

			if(!toggle--) {
				break;
			}
		}

		if(!handle.in_use && handle.num_blocks >= num) {
			ptr = handle.ptr;
			handle.in_use = true;
			handle.num_blocks = num - 1;
			toggle = num-1;
		}
	}

	return ptr;
}

void CacheManager::free_raw(PcmSample* ptr) {
	
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
	auto nblocks = m_num_blocks - 1u;
	
	for(auto i = 0u; i < m_num_blocks; i++) {

		m_handles.push_back(CacheHandle {
				.ptr = (ptr + (i * m_block_size)),
				.in_use = false,
				.num_blocks = nblocks--
		});
	}
}

#if DEVBUILD
const std::vector<CacheHandle> & CacheManager::get_const_handles() const {
	return m_handles;

}
#endif