#include "framework/memory/cache_ptr.hpp"

using namespace strangeio::memory;
cache_ptr::cache_ptr() :
m_cache(nullptr), m_block(nullptr), m_num_blocks(0)
{}

cache_ptr::cache_ptr(const PcmSample* block, unsigned int num_blocks, cache_utility* cache) :
m_cache(cache), m_num_blocks(num_blocks)
{ 
	m_block = const_cast<PcmSample*>(block);
}

cache_ptr::cache_ptr(cache_ptr& that) {
	m_cache = that.m_cache;
	m_block = that.m_block;
	m_num_blocks = that.m_num_blocks;

	that.m_block = nullptr;
	that.m_num_blocks = 0;
}

cache_ptr::cache_ptr(cache_ptr&& that) {
	m_cache = that.m_cache;
	m_block = that.m_block;
	m_num_blocks = that.m_num_blocks;

	that.m_block = nullptr;
	that.m_num_blocks = 0;
}

cache_ptr::~cache_ptr() {
	if(m_block == nullptr || m_cache == nullptr) return;
	m_cache->free_raw(m_block);
}

unsigned int cache_ptr::block_size() const {

	return m_cache ? m_cache->block_size() : 0;
}

unsigned int cache_ptr::num_blocks() const {
	return m_num_blocks;
}

const PcmSample* cache_ptr::release() {
	auto ptr = m_block;
	m_num_blocks = 0;
	m_block = nullptr;
	return ptr;
}

const PcmSample* cache_ptr::get() const {
	return m_block;
}

void cache_ptr::reset(const PcmSample* ptr, unsigned int num_blocks) {
	if(m_block != nullptr) {
		m_cache->free_raw(m_block);
	}

	m_num_blocks = num_blocks;
	m_block = const_cast<PcmSample*>(ptr);
}

void cache_ptr::swap(cache_ptr& cptr) {
	auto nblocks = cptr.num_blocks();
	auto ptr = cptr.release();
	
	cptr.reset(release(), m_num_blocks);
	reset(ptr, nblocks);
}

const PcmSample* cache_ptr::operator *() const {
	return m_block;
}

PcmSample& cache_ptr::operator [](int index) {
	return m_block[index];
}

cache_ptr& cache_ptr::operator =(cache_ptr& that) {

		m_cache = that.m_cache;
		m_block = that.m_block;
		m_num_blocks = that.m_num_blocks;

		that.m_block = nullptr;
		that.m_num_blocks = 0;
		return *this;

}

cache_ptr::operator bool() const  {
	if(!m_block) return false;
	return true;
}