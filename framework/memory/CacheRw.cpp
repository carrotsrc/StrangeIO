#include "framework/memory/CacheRw.hpp"

using namespace strangeio::Memory;
CacheRw::CacheRw() :
m_cache(nullptr)
{}

void CacheRw::set_cache_utility(CacheUtilityInterface* cache) {
	m_cache = cache;
}

CachePtr CacheRw::cache_alloc(unsigned int num) {
	if(!m_cache) {
		return CachePtr(nullptr,0,nullptr);
	}

	auto ptr = m_cache->alloc_raw(num);
	
	return CachePtr(ptr,num,m_cache);
}