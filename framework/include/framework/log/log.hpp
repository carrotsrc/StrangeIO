#ifndef LOG_HPP
#define LOG_HPP
#include <iostream>
#include <sstream>


namespace strangeio {

class log {
	log() : m_stream(std::cerr) { }
	
	std::ostream& m_stream;
	std::stringstream m_ss;

public:
	
	static log& inst() {
		static log l;
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

