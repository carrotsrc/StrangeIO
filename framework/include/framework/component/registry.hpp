#ifndef SYS_REGISTRY_HPP
#define SYS_REGISTRY_HPP
#include <vector>
#include <mutex>

#include "framework/component/registry_utility.hpp"

namespace strangeio {
namespace component {	


/**
 * The central registry of components
 */
class registry : public registry_utility {
public:
	registry();
	registry(const registry& orig) = delete;
	virtual ~registry();

	/**
	 * Register a component with the registry
	 * 
	 * The component must be of type registrable
	 * @param cmpt Registrable to register
	 */
	void register_component(registrable& cmpt);
	
	/**
	 * Iterator to start of registry
	 * @return A const iterator of type rhandle
	 */
	std::vector<rhandle>::const_iterator begin();
	
	/**
	 * Iterator to end of registry
	 * @return  A const iterator of type rhandle
	 */
	std::vector<rhandle>::const_iterator end();
	
	/**
	 * Get handle of component with ID
	 * @param id long ID of unit
	 * @return rhandle value of component
	 */
	rhandle with_id(long id);
	
private:
	std::mutex m_mutex;
	long m_current_id;
	std::vector<rhandle> m_registry; 
};

}
}
#endif /* SYS_REGISTRY_HPP */

