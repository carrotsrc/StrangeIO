#include "framework/memory/CachePtr.hpp"

using namespace StrangeIO::Memory;

CachePtr::CachePtr(const PcmSample* block, CacheUtilityInterface* cache) :
m_cache(cache)
{ 
	m_block = const_cast<PcmSample*>(block);
}

CachePtr::~CachePtr() {
	if(m_block == nullptr) return;

	m_cache->free_raw(m_block);
}

const PcmSample* CachePtr::release() {
	auto ptr = m_block;
	m_block = nullptr;
	return ptr;
}

const PcmSample* CachePtr::get() const {
	return m_block;
}

void CachePtr::reset(const PcmSample* ptr) {
	if(m_block != nullptr) {
		m_cache->free_raw(m_block);
	}

	m_block = const_cast<PcmSample*>(ptr);
}

void CachePtr::swap(CachePtr& cptr) {
	auto ptr = cptr.release();
	cptr.reset(release());
	reset(ptr);
}