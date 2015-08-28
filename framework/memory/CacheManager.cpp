#include "framework/memory/CacheManager.hpp"

PcmSample* CacheManager::alloc_raw(int num) {
	
}
size_t CacheManager::cache_size() const {
	return m_cache_size;
}

int CacheManager::num_blocks() const {
	return m_num_blocks;
}
int CacheManager::block_size() const {
	return m_block_size;
}
