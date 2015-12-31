#include <iostream>
#include "framework/buffer/circular.hpp"

using namespace strangeio::buffer;

circular::circular()
	: m_size(0)
	, m_load(0)
	, m_overwrite(false)
	, m_buffer(nullptr)
	, m_write(nullptr)
	, m_read(nullptr)
	, m_end(nullptr)
{ }

circular::circular(const circular& orig) {
}

circular::~circular() {
}

void circular::set_size(int size) {
	if(m_buffer) {
		delete m_buffer;
	}
	
	m_buffer = new PcmSample[size];
	m_write = m_read = m_buffer;
	m_end = m_buffer + size;
	m_size = size;
}

int circular::size() const {
	return m_size;
}

void circular::set_overwrite(bool toggle) {
	m_overwrite = toggle;
}

bool circular::overwrite() const {
	return m_overwrite;
}

bool circular::insert(PcmSample* samples, unsigned int num) {
	if(!m_size) return false;
	if(!m_overwrite &&  num > (m_size - m_load) ) return false;
	
	for(auto i = 0u; i < num; i++) {
		*(m_write) = samples[i];
		if(++m_write == m_end) m_write = m_buffer;
	}
	
	m_load += num;
	return true;
}

bool circular::get(strangeio::memory::cache_ptr & cptr) {
	if(!m_size) return false;
	auto bs = cptr.block_size();
	
	if(!m_overwrite && m_load < bs) return false;
	
	for(auto i = 0u; i < bs ; i++) {
		cptr[i] = *(m_read);
		if(++m_read == m_end) m_read = m_buffer;
	}
	
	m_load -= bs;
	
	
	return true;
}