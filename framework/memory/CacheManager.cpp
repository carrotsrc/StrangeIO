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
PcmSample* CacheManager::alloc_raw(int num) {
	if(m_cache_size == 0) return nullptr;

	/*
	for(auto& hdnl : m_handles)

	CacheHandle hndl = m_handles.pop_back();
	std::cout << "Start of heap: " << m_raw_cache << std::endl;
	std::cout << "CacheHandle.ptr =" << hndl.ptr << std::endl;
	std::cout << "CacheHandle.num_blocks =" << hndl.num_blocks << std::endl;
	
	return hndl.ptr;
	 */

	return nullptr;
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

void CacheManager::cache_alloc(int block_size) {
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