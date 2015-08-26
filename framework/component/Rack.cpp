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
		std::pair<std::string, unit_uptr>(
			label, std::move(unit)
		)
	);
}
