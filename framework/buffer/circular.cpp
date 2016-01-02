#include <iostream>

#include "framework/buffer/circular.hpp"

using namespace strangeio::buffer;

circular::circular()
	: m_size(0)
	, m_load(0)
	, m_start(0)
	, m_end(0)
	, m_overwrite(false)
	, m_buffer(nullptr)
{ }

circular::~circular() {
}
#include <iostream>
#include <cstdio>
void circular::set_size(int size) {
	if(m_buffer) {
		delete[] m_buffer;
	}
	
	m_size = size;
	m_buffer = new PcmSample[size];
	m_load = 0;
	m_start = m_end = 0;
	
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

	for(auto i = 0u; i < num; i++) {
		m_buffer[m_end] = samples[i];
		m_end = (m_end + 1) % m_size;
		m_load++;
	}
	//std::cout << "Load: " << m_load << std::endl;

	return true;
}

bool circular::get(PcmSample* out, unsigned int num) {
	if(!m_size) return false;
	
	for(auto i = 0u; i < num; i++) {
		out[i] = m_buffer[m_start];
		m_start = (m_start+1) % m_size;
		m_load--;
	}
	//std::cout << "Unload: " << m_load << std::endl;

	return true;
}

int circular::load() const {
	return m_load;
}