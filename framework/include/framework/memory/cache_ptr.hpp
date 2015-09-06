#ifndef __CACHE_PTR_HPP_1440753730__
#define __CACHE_PTR_HPP_1440753730__
#include "framework/fwcommon.hpp"
#include "framework/memory/cache_utility.hpp"
namespace strangeio {
namespace memory {

class cache_ptr {
public:
	cache_ptr();
	cache_ptr(const PcmSample* block, unsigned int num_blocks, cache_utility* cache);
	cache_ptr(cache_ptr& that);
	cache_ptr(cache_ptr&& that);
	~cache_ptr();

	unsigned int num_blocks() const;
	unsigned int block_size() const;

	const PcmSample* release();
	const PcmSample* get() const;
	void reset(const PcmSample* ptr, unsigned int num_blocks = 1);
	void swap(cache_ptr& cptr);

	void copy_from(PcmSample* samples, unsigned int num_samples);
	void copy_to(PcmSample* samples);

	const PcmSample* operator *() const;
	PcmSample& operator [](int index);
	cache_ptr& operator =(cache_ptr& that);
	operator bool() const;

private:
	cache_utility* m_cache;
	PcmSample* m_block;
	unsigned int m_num_blocks;
};


}
}
#endif
