#ifndef __CACHE_PTR_HPP_1440753730__
#define __CACHE_PTR_HPP_1440753730__
#include "framework/fwcommon.hpp"
#include "framework/memory/cache_utility.hpp"

#if CACHE_TRACKING
	#include "framework/component/registrable.hpp"
#endif

namespace strangeio {
namespace memory {

class cache_ptr {
public:
	cache_ptr();
#if CACHE_TRACKING
	cache_ptr(PcmSample* block, unsigned int num_blocks, cache_utility* cache, long id);
#else
	cache_ptr(PcmSample* block, unsigned int num_blocks, cache_utility* cache);
#endif
	
	cache_ptr(cache_ptr& that) = delete;
	cache_ptr(cache_ptr&& that);
	~cache_ptr();

	unsigned int num_blocks() const;
	unsigned int block_size() const;

	PcmSample* release();
	PcmSample* get();
	void reset(const PcmSample* ptr, unsigned int num_blocks = 1);
	void swap(cache_ptr& cptr);

	void copy_from(PcmSample* samples, unsigned int num_samples);
	void copy_to(PcmSample* samples);
        void copy_to(PcmSample* samples, unsigned int num_samples);

	void free();

	PcmSample* operator *();
	PcmSample& operator [](int index);
	cache_ptr& operator =(cache_ptr&& that);
	operator bool() const;
	
#if CACHE_TRACKING
	long tracking_id();
	void set_owner(strangeio::component::rhandle handle);
	strangeio::component::rhandle owner();
	
#endif
private:
	cache_utility* m_cache;
	PcmSample* m_block;
	unsigned int m_num_blocks;
#if CACHE_TRACKING
	long m_tracking_id;
	strangeio::component::rhandle m_owner;
	
	void reset_owner();
#endif
};


}
}
#endif
