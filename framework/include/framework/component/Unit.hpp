#ifndef __UNIT_HPP_1440406068__
#define __UNIT_HPP_1440406068__

#include "framework/component/Component.hpp"

namespace StrangeIO {
namespace Component {

class Unit {
public:
	Unit(UnitType utype, std::string umodel, std::string ulabel);
	
	UnitType utype() const;
	std::string umodel() const;
	std::string ulabel() const;

	ComponentState cstate() const;

protected:

private:
	const UnitType m_utype;
	const std::string m_umodel, m_ulabel;

	ComponentState m_cstate;

};

} // Component
} // StrangeIO
#endif

