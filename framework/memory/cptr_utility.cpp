#include "framework/memory/cptr_utility.hpp"

using namespace strangeio::memory;
cptr_utility::cptr_utility() :
m_cache(nullptr)
{}

void cptr_utility::set_cache_utility(cache_utility* cache) {
	m_cache = cache;
}

cache_ptr cptr_utility::cache_alloc(unsigned int num) {
	if(!m_cache) {
		return cache_ptr(nullptr,0,nullptr);
	}

	auto ptr = m_cache->alloc_raw(num);
	
	return cache_ptr(ptr,num,m_cache);
}