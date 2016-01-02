#ifndef CIRCULAR_HPP
#define CIRCULAR_HPP

#include "framework/fwcommon.hpp"
#include "framework/memory/cache_ptr.hpp"

namespace strangeio {
namespace buffer {

class circular {
public:
	circular();
	virtual ~circular();

	void set_size(int size);
	int size() const;
	
	void set_overwrite(bool toggle);
	bool overwrite() const;
        
        int load() const;
	
	bool insert(PcmSample* samples, unsigned int num);
	
	bool get(PcmSample* out, unsigned int num);

private:
	
	unsigned int m_size, m_start, m_end;
        signed int m_load;
	
	bool m_overwrite;
	
	PcmSample *m_buffer;
};

}
}
#endif /* CIRCULAR_HPP */

