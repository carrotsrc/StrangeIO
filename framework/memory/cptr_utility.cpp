#include "framework/memory/cptr_utility.hpp"

using namespace strangeio::memory;
cptr_utility::cptr_utility() :
m_cache(nullptr)
{}

void cptr_utility::set_cache_utility(cache_utility* cache) {
	m_cache = cache;
}

cache_ptr cptr_utility::cache_alloc(unsigned int num) const {
	if(!m_cache) {
		return cache_ptr(nullptr,0,nullptr);
	}

	auto ptr = m_cache->alloc_raw(num);
	
	return cache_ptr(ptr,num,m_cache);
}

void cptr_utility::set_utility(cptr_utility& ref) const {
	ref.set_cache_utility(m_cache);
}

unsigned int cptr_utility::block_size() const {
	if(!m_cache) return 1;
	return m_cache->block_size();
}