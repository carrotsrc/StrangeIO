#include "framework/component/Unit.hpp"

using namespace StrangeIO::Component;

Unit::Unit(UnitType utype, std::string umodel, std::string ulabel) : 
m_utype(utype), m_umodel(umodel), m_ulabel(ulabel), m_cstate(Inactive)
{ }

UnitType Unit::utype() const {
	return m_utype;
}

std::string Unit::umodel() const {
	return m_umodel;
}

std::string Unit::ulabel() const {
	return m_ulabel;
}

ComponentState Unit::cstate() const {
	return m_cstate;
}
