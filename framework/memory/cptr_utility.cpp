#include "framework/memory/cptr_utility.hpp"

using namespace strangeio::memory;
cptr_utility::cptr_utility(std::string component)
	: m_cache(nullptr)
	, m_component_name(component)

{}

void cptr_utility::set_cache_utility(cache_utility* cache) {
	m_cache = cache;
}

#if CACHE_TRACKING
cache_ptr cptr_utility::cache_alloc(unsigned int num) {
	if(!m_cache) {
		return std::move(cache_ptr(nullptr,0,nullptr,0));
	}
	
	auto id = 0l;
	try {
		auto ptr = m_cache->alloc_raw(num, &id);
		return std::move(cache_ptr(ptr,num,m_cache, id));
	} catch(strangeio::cache_drain& e) {
		e.set_component(m_component_name);
		throw e;
	}
}
#else
cache_ptr cptr_utility::cache_alloc(unsigned int num) const {
	if(!m_cache) {
		return std::move(cache_ptr(nullptr,0,nullptr));
	}
	
	try {
		auto ptr = m_cache->alloc_raw(num);
		return std::move(cache_ptr(ptr,num,m_cache));
	} catch(strangeio::cache_drain& e) {
		e.set_component(m_component_name);
		throw e;
	}
}
#endif
void cptr_utility::set_utility(cptr_utility& ref) const {
	ref.set_cache_utility(m_cache);
}

unsigned int cptr_utility::block_size() const {
	if(!m_cache) return 1;
	return m_cache->block_size();
}