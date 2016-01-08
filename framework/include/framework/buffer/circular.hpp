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
	typedef T		value_type;
	typedef size_t	size_type;
	
public:
	circular()
		: m_start(0), m_end(0)
		, m_size(0), m_capacity(0)
		, m_buffer(nullptr)
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
	
    void push_back(value_type& item) {
		m_buffer[increment()] = item;
	}
	
	void push_back(value_type&& item) {
		m_buffer[increment()] = item;
	}
	
	void push_many(value_type* items, size_t count) {
		for(auto i = 0u; i < count; i++) {
			m_buffer[increment()] = items[i];
		}
	}
	
	value_type& front() const noexcept {
		return m_buffer[m_start];
	}
	
	value_type&& move_front() noexcept {
		return std::move(m_buffer[decrement()]);
	}
	
	void pop_front() {
		if(!m_size) return;
		decrement();
	}
	
	void pop_many(value_type* dest, size_type count) {
		if(!m_size) return;
		
		for(auto i = 0u; i < count; i++) {
			dest[i] = m_buffer[decrement()];
		}
	}
	
	void move_many(value_type* dest, size_type count) {
		if(!m_size) return;

		for(auto i = 0u; i < count; i++) {
			dest[i] = std::move(m_buffer[decrement()]);
		}
	}

	value_type& at(size_type pos) const {
		if(pos > m_size) throw std::out_of_range("Position out of range [siobuf::ciruclar]");
		return m_buffer[pos];
	}
	
	bool empty() const noexcept {
		return m_size == 0;
	}
	
	void clear() noexcept {
		m_size = m_start = m_end = 0;
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

template class circular<PcmSample>;
}
}
#endif /* CIRCULAR_HPP */

