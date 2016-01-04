#include "framework/memory/cache_ptr.hpp"

using namespace strangeio::memory;
cache_ptr::cache_ptr() 
	: m_cache(nullptr)
	, m_block(nullptr)
	, m_num_blocks(0)
{}

cache_ptr::cache_ptr(const PcmSample* block, unsigned int num_blocks, cache_utility* cache) 
	: m_cache(cache)
	, m_num_blocks(num_blocks)
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

PcmSample* cache_ptr::release() {
	auto ptr = m_block;
	m_num_blocks = 0;
	m_block = nullptr;
	return ptr;
}

PcmSample* cache_ptr::get() {
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

PcmSample* cache_ptr::operator *() {
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

void cache_ptr::copy_from(PcmSample* samples, unsigned int num_samples) {
	if(!m_block) return;
	std::copy(samples, samples+num_samples, m_block);
}

void cache_ptr::copy_to(PcmSample* samples){
	if(!m_block) return;
	auto len = m_num_blocks * block_size();
	std::copy(m_block, m_block+len, samples);
}

void cache_ptr::copy_to(PcmSample* samples, unsigned int num_samples) {
	if(!m_block) return;
	std::copy(m_block, m_block+num_samples, samples);
} 

void cache_ptr::free() {
	if(!m_block || !m_cache) return;
	m_cache->free_raw(m_block);
	m_block = nullptr;
}