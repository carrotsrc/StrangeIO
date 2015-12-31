#ifndef CIRCULAR_HPP
#define CIRCULAR_HPP

#include "framework/fwcommon.hpp"
#include "framework/memory/cache_ptr.hpp"

namespace strangeio {
namespace buffer {

class circular {
public:
	circular();
	circular(const circular& orig);
	virtual ~circular();

	void set_size(int size);
	int size() const;
	
	void set_overwrite(bool toggle);
	bool overwrite() const;
	
	bool insert(PcmSample* samples, unsigned int num);
	
	bool get(strangeio::memory::cache_ptr & cptr);

private:
	
	unsigned int m_size, m_load;
	
	bool m_overwrite;
	
	PcmSample *m_buffer, *m_write, *m_read, *m_end;

};

}
}
#endif /* CIRCULAR_HPP */

