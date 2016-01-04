#ifndef CIRCULAR_HPP
#define CIRCULAR_HPP

#include "framework/fwcommon.hpp"
#include "framework/memory/cache_ptr.hpp"
#include <mutex>
namespace strangeio {
namespace buffer {

class circular {
public:
	circular();
	virtual ~circular();

	void set_size(int size);
	int size() const;
        
        int load() const;
	
	bool insert(PcmSample* samples, unsigned int num);
	
	bool get(PcmSample* out, unsigned int num);

private:
    std::mutex m_mutex;
    unsigned int m_size, m_start, m_end;
    signed int m_load;
	
    PcmSample* m_buffer, *m_ptr;
};

}
}
#endif /* CIRCULAR_HPP */

