#ifndef REGISTRY_UTILITY_HPP
#define REGISTRY_UTILITY_HPP
#include "framework/component/registrable.hpp"

namespace strangeio {
namespace component {

	struct regitem {
		long id;
		ctype type;
		std::string model;
		std::string label;
		bool active;
	};
	
	class registry_utility {
	public:
		virtual void register_component(registrable& cmpt) = 0;
		virtual std::vector<regitem>::const_iterator begin() = 0;
		virtual std::vector<regitem>::const_iterator end() = 0;
		virtual regitem with_id(long id) = 0;
	};	
}
}


#endif /* REGISTRY_UTILITY_HPP */

