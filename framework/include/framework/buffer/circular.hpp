#ifndef CIRCULAR_HPP
#define CIRCULAR_HPP

#include <cstddef>

#include "framework/fwcommon.hpp"
#include "framework/memory/cache_ptr.hpp"

namespace strangeio {
namespace buffer {

template<typename T>
class circular {
public:
	typedef T				value_type;
	typedef size_t			size_type;
	typedef value_type&		reference;
	typedef value_type*		pointer;
	
public:
	circular()
		: m_start(0), m_end(0)
		, m_size(0), m_capacity(0)
		, m_buffer(nullptr)
		{ };

	circular(size_type capacity)
		: m_start(0), m_end(0)
		, m_size(0), m_capacity(capacity)
		, m_buffer(new value_type[capacity])
		{ };

	virtual ~circular() { 
		if(m_buffer) delete[] m_buffer; 
	};

	void reserve(size_type new_cap) noexcept {

		if(new_cap <= m_capacity) return;

		auto t = new T[new_cap];
		
		if(m_size)
			std::copy(m_buffer, m_buffer+m_size, t);
		
		m_buffer = t;
		m_capacity = new_cap;
	}

	size_type capacity() const noexcept {
		return m_capacity;
	};
	
	size_type size() const {
		return m_size;
	}
	
    void push_back(reference item) {
		m_buffer[increment()] = item;
	}
	
	void push_back(value_type&& item) {
		m_buffer[increment()] = std::move(item);
	}
	
	void push_many(pointer items, size_t count) {
		for(auto i = 0u; i < count; i++) {
			m_buffer[increment()] = items[i];
		}
	}
	
	reference front() const noexcept {
		return m_buffer[m_start];
	}
	
	value_type&& move_front() noexcept {
		return std::move(m_buffer[decrement()]);
	}
	
	void pop_front() {
		if(!m_size) return;
		decrement();
	}
	
	void pop_many(pointer dest, size_type count) {
		if(!m_size) return;
		
		for(auto i = 0u; i < count; i++) {
			dest[i] = m_buffer[decrement()];
		}
	}
	
	void move_many(pointer dest, size_type count) {
		if(!m_size) return;

		for(auto i = 0u; i < count; i++) {
			dest[i] = std::move(m_buffer[decrement()]);
		}
	}

	reference at(size_type pos) const {
		if(pos > m_size) throw std::out_of_range("Position out of range [siobuf::ciruclar]");
		auto p = m_start + pos % m_capacity;
		return m_buffer[p];
	}
	
	bool empty() const noexcept {
		return m_size == 0;
	}
	
	void clear() noexcept {
		m_size = m_start = m_end = 0;
	}
	
	
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

typedef circular<PcmSample> circular_pcm;
typedef circular<strangeio::memory::cache_ptr> circular_cptr;

}
}
#endif /* CIRCULAR_HPP */

