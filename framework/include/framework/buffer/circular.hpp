#ifndef CIRCULAR_HPP
#define CIRCULAR_HPP

#include <cstddef>

#include "framework/fwcommon.hpp"
#include "framework/memory/cache_ptr.hpp"

namespace strangeio {
namespace buffer {

/**
 * A buffer without a fixed start/end, allows overwriting when full
 *
 * This buffer is useful for storing streaming information.. It has capacity for
 * a specified number of items. Once the buffer is full, adding a new item will
 * overwrite the oldest item of data.
 * 
 * It's capacity describes how many items it can store while it's size describes
 * how many items it current hasly stored.
 */
template<typename T>
class circular {
public:
	typedef T				value_type;
	typedef size_t			size_type;
	typedef value_type&		reference;
	typedef value_type*		pointer;
	
public:
	/**
	 * Default construct with zero capacity
	 */
	circular()
		: m_start(0), m_end(0)
		, m_size(0), m_capacity(0)
		, m_buffer(nullptr)
		{ };

	/**
	 * Construct with specified capacity 
	 * 
	 * @param capacity The capacity of the buffer
	 */
	circular(size_type capacity)
		: m_start(0), m_end(0)
		, m_size(0), m_capacity(capacity)
		, m_buffer(new value_type[capacity])
		{ };

	virtual ~circular() { 
		if(m_buffer) delete[] m_buffer; 
	};

	/**
	 * Resize the buffer with a reserve capacity
	 * 
	 * @param new_cap The new capacity
	 */
	void reserve(size_type new_cap) noexcept {

		if(new_cap <= m_capacity) return;

		auto t = new T[new_cap];
		
		if(m_size)
			std::copy(m_buffer, m_buffer+m_size, t);
		
		m_buffer = t;
		m_capacity = new_cap;
	}

	/** Get the current capacity
	 * 
	 *  @return size_type
	 */
	size_type capacity() const noexcept {
		return m_capacity;
	};
	
	/**
	 * Get the current size (fill)
	 * 
	 * @return size_type
	 */
	size_type size() const {
		return m_size;
	}
	
	/**
	 * Push an item onto the end of the buffer
	 * 
	 * This method increments size
	 * @param item Item of type T
	 */
    void push_back(reference item) {
		m_buffer[increment()] = item;
	}
	
	/**
	 * Move an RValue into the buffer
	 * 
	 * This method increments size
	 * @param item RValue reference of type T
	 */
	void push_back(value_type&& item) {
		m_buffer[increment()] = std::move(item);
	}
	
	/**
	 * Push many of type T
	 * 
	 * This method increments size by count
	 * @param items A pointer to contiguous of type T
	 * @param count The number of items pointed to
	 */
	void push_many(pointer items, size_t count) {
		for(auto i = 0u; i < count; i++) {
			m_buffer[increment()] = items[i];
		}
	}
	
	/**
	 * Get reference to first item in buffer
	 * 
	 * This method has no effect on size
	 * @return A reference f type T
	 */
	reference front() const noexcept {
		return m_buffer[m_start];
	}
	
	/**
	 * Move front out of buffer as RValue
	 * 
	 * This method decrements size
	 * @return RValue reference
	 */
	value_type&& move_front() noexcept {
		return std::move(m_buffer[decrement()]);
	}
	
	/**
	 * Pop the first item off of the buffer
	 * 
	 * This method decrements size
	 */
	void pop_front() {
		if(!m_size) return;
		decrement();
	}
	
	/**
	 * Pop many items out of the buffer into destination
	 * 
	 * This method decrements size by count
	 * 
	 * @param dest A pointer to memory
	 * @param count The number of items to pop
	 */
	void pop_many(pointer dest, size_type count) {
		if(!m_size) return;
		
		for(auto i = 0u; i < count; i++) {
			dest[i] = m_buffer[decrement()];
		}
	}
	
	/**
	 * Move a range of RValue items out of the buffer
	 * 
	 * This method decrements size by count
	 * @param dest The pointer to the destination
	 * @param count The number of items
	 */
	void move_many(pointer dest, size_type count) {
		if(!m_size) return;

		for(auto i = 0u; i < count; i++) {
			dest[i] = std::move(m_buffer[decrement()]);
		}
	}

	/**
	 * Get reference at specified position relative to start
	 * 
	 * This method has no effect on size
	 * @throws out_of_range
	 * @param pos Position
	 * @return Reference of type T
	 */
	reference at(size_type pos) const {
		if(pos > m_size) throw std::out_of_range("Position out of range [siobuf::ciruclar]");
		auto p = m_start + pos % m_capacity;
		return m_buffer[p];
	}
	
	/**
	 * Check whether buffer is empty
	 * @return boolean true if empyt; otherwise false
	 */
	bool empty() const noexcept {
		return m_size == 0;
	}
	
	/**
	 * Reset positions to clear buffer
	 */
	void clear() noexcept {
		m_size = m_start = m_end = 0;
	}
	
	/**
	 * Reference at position specified, relative to start
	 * 
	 * This method has no effect on size
	 * @throws out_of_range
	 * @param pos Item position
	 * @return Reference to item
	 */
	reference operator[] (size_type pos) {
		return at(pos);
	}

private:
	unsigned int m_start, m_end;
	size_type m_size, m_capacity;
	
    value_type* m_buffer;
	
	size_type increment() {
		auto n = m_end++;
		m_end = m_end % m_capacity;
		if(++m_size > m_capacity) m_size = m_capacity;
		return n;
	}

	size_type decrement() {
		auto n = m_start++;
		m_start = m_start % m_capacity;
		m_size--;
		return n;
	}
};

/**
 * Circular buffer for samples
 */
typedef circular<PcmSample> circular_pcm;

/**
 * Circular buffer for cache_ptrs
 */
typedef circular<strangeio::memory::cache_ptr> circular_cptr;

}
}
#endif /* CIRCULAR_HPP */

