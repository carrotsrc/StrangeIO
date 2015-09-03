#ifndef __UNITLOADER_HPP_1441029038__
#define __UNITLOADER_HPP_1441029038__

#include "framework/fwcommon.hpp"
#include "framework/component/component.hpp"
#include "framework/component/unit.hpp"

namespace strangeio {
namespace component {

class unit_loader {
public:
	unit_uptr load(std::string model, std::string label, std::string lib);
};

} // Component
} // StrangeIO
 
#endif