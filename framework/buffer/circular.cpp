#include "framework/buffer/circular.hpp"

using namespace strangeio::buffer;

circular::circular()
	: m_size(0)
	, m_load(0)
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

int circular::size() {
	return m_size;
}

bool circular::insert(PcmSample* samples, int num) {
	if(!m_size) return false;
	auto diff = m_end - m_write;
	if(num < diff) {
		std::copy(samples, samples+num, m_write);
		m_write += num;
	} else {
		auto nblock = samples + diff;
		std::copy(samples, nblock, m_write);
		m_write = m_buffer;
		num -= diff;
		std::copy(nblock, nblock+num, m_write);
	}
	
	m_write += num;
	return true;
}

bool circular::insert(strangeio::memory::cache_ptr cptr, int num) {
	if(!m_size) return false;
	
	return true;
}

bool circular::get(strangeio::memory::cache_ptr & cptr) {
	if(!m_size) return false;
	
	return true;
}