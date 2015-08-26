#include "framework/component/Rack.hpp"
using namespace StrangeIO::Component;
Rack::Rack()
{ }

Rack::~Rack() {

}

void Rack::add_unit(unit_uptr unit) {
	unit->set_rack(this);
	auto label = unit->ulabel();
	m_mounted.insert(
		std::pair<std::string, unit_sptr>(
			label, unit_sptr(std::move(unit))
		)
	);
}

const std::map<std::string, unit_sptr> & Rack::get_units() {
	return m_mounted;
}

unit_wptr Rack::get_unit(std::string label) {

	auto u = m_mounted.find(label);

	if(u == m_mounted.end()) {
		return unit_wptr();
	}

	return u->second;
}


