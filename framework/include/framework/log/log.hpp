#ifndef LOG_HPP
#define LOG_HPP
#include <iostream>
#include <sstream>
#include <mutex>

namespace strangeio {

class log {
	log() : m_stream(std::cerr) { }
	
	std::ostream& m_stream;
	std::stringstream m_ss;
	std::mutex m_mutex;

public:
	
	static log& inst() {
		static log l;
		l.m_mutex.lock();
		return l;
	};
	
	template <class T>
	log& operator<< (const T& v) {
		m_ss << v;
		return *this;
	};
	
	log& flush() {
		m_stream << m_ss.str() << std::flush;
		m_ss.str("");
		m_mutex.unlock();
		return *this;
	}
	log& operator<< (log& (*manip)(log&)) {
		(*manip)(*this);
		
		return *this;
	}
};	


inline log&
lflush(log& l)
{ return l.flush(); };

inline log&
lendl(log& l)
{ l << "\n"; return l.flush(); };

}
#endif /* LOG_HPP */

