#ifndef __UNITLOADER_HPP_1441029038__
#define __UNITLOADER_HPP_1441029038__

#include "framework/fwcommon.hpp"
#include "framework/component/Component.hpp"
#include "framework/component/Unit.hpp"

namespace StrangeIO {
namespace component {

class UnitLoader {
	unit_uptr load(std::string unit, std::string name, std::string library);
};

} // Component
} // StrangeIO
 
#endif