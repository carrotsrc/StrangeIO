#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP
#include <exception>

namespace strangeio {
 
    class internal_exception : std::exception {
	protected:
		char m_msg[256];
		std::string m_component;
    public:
		internal_exception()
			: m_component("framework")
		{}
			
		internal_exception(std::string cmpt)
			: m_component(cmpt)
		{}
		
        void set_component(std::string cmpt) {
			m_component = cmpt;
		};

        virtual const char* what() {
			return "Generic internal exception";
		}
    };

    class cache_drain : public internal_exception {
    public:
		cache_drain() : internal_exception() { }
		cache_drain (std::string cmpt) : internal_exception(cmpt) { }
        virtual const char* what() {
			
			std::sprintf(m_msg, "Exception: Cache drained of resources - cannot allocate block\t[%s]", m_component.c_str());
			
			return m_msg;
		}
  
    };
	
    class cache_nullptr : public internal_exception {
    public:
		cache_nullptr() : internal_exception() { }
		cache_nullptr (std::string cmpt) : internal_exception(cmpt) { }
		
        virtual const char* what() {
			std::sprintf(m_msg, "Exception: Null cache_ptr\t[%s]", m_component.c_str());	
			return m_msg;
		}
  
    };
}
#endif /* EXCEPTIONS_HPP */

