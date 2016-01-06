#ifndef SYS_REGISTRY_HPP
#define SYS_REGISTRY_HPP
#include <vector>
#include <mutex>

#include "framework/component/registry_utility.hpp"

namespace strangeio {
namespace component {	


class registry : public registry_utility {
public:
	registry();
	registry(const registry& orig) = delete;
	virtual ~registry();

	void register_component(registrable& cmpt);
	std::vector<rhandle>::const_iterator begin();
	std::vector<rhandle>::const_iterator end();
	rhandle with_id(long id);
	
private:
	std::mutex m_mutex;
	long m_current_id;
	std::vector<rhandle> m_registry; 
};

}
}
#endif /* SYS_REGISTRY_HPP */

