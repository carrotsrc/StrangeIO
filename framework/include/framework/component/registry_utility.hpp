#ifndef REGISTRY_UTILITY_HPP
#define REGISTRY_UTILITY_HPP
#include "framework/component/registrable.hpp"

namespace strangeio {
namespace component {	
	class registry_utility {
	public:
		virtual void register_component(registrable& cmpt) = 0;
		virtual std::vector<rhandle>::const_iterator begin() = 0;
		virtual std::vector<rhandle>::const_iterator end() = 0;
		virtual rhandle with_id(long id) = 0;
	};	
}
}


#endif /* REGISTRY_UTILITY_HPP */

