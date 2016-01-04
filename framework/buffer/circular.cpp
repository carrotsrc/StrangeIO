#include <iostream>
#include <memory>

#include "framework/buffer/circular.hpp"

using namespace strangeio::buffer;

circular::circular()
	: m_size(0)
	, m_start(0)
	, m_end(0)
	, m_load(0)
	, m_buffer(nullptr)
	, m_ptr(nullptr)
{ }

circular::~circular() {
	if(m_size > 0) {
		delete[] m_ptr;
	}
}

void circular::set_size(int size) {
	if(m_size > 0) {
		delete[] m_ptr;
	}
	
	m_size = size;
	auto tsize = sizeof(PcmSample);
	auto bsize = tsize + size;
	
	m_ptr = new PcmSample[bsize];

	if(reinterpret_cast<uintptr_t>(m_ptr) % alignof(PcmSample) == 0){
		m_buffer = m_ptr;
	} else {
		auto vptr = reinterpret_cast<void*>(m_ptr);
		std::align(alignof(PcmSample), sizeof(PcmSample), vptr, bsize);
		m_buffer = reinterpret_cast<PcmSample*>(vptr);
	}

	m_load = 0;
	m_start = m_end = 0;
	
}

int circular::size() const {
	return m_size;
}

bool circular::insert(PcmSample* samples, unsigned int num) {
	if(!m_size) return false;
	m_mutex.lock();
	for(auto i = 0u; i < num; i++) {
		m_buffer[m_end] = samples[i];
		m_end = (m_end + 1) % m_size;
		m_load++;
	}
	m_mutex.unlock();
	return true;
}

bool circular::get(PcmSample* out, unsigned int num) {
	if(!m_size) return false;
	m_mutex.lock();
	for(auto i = 0u; i < num; i++) {
		out[i] = m_buffer[m_start];
		
		m_start = (m_start+1) % m_size;
		m_load--;
	}
	m_mutex.unlock();
	return true;
}

int circular::load() const {
	return m_load;
}