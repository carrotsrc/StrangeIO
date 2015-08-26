#include "framework/component/Rack.hpp"
using namespace StrangeIO::Component;
Rack::Rack() :
m_resync(false)
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

void Rack::clear_units() {
	m_mounted.clear();
}

void Rack::add_mainline(std::string name) {
	if(m_mainlines.find(name) != m_mainlines.end())
		return;

	m_mainlines.insert(
			std::pair<std::string, unit_wptr>(
				name, unit_wptr()
			)
		);
}

bool Rack::connect(std::string mainline, std::string unit) {
	auto line = m_mainlines.find(mainline);

	if(line == m_mainlines.end()) return false;

	auto wptr = get_unit(unit);
	if(wptr.expired()) return false;

	line->second = wptr;

	return true;
}

void Rack::toggle_resync() {
	m_resync = true;
}


void Rack::cycle(CycleType type) {
	for( auto& wptr : m_mainlines ) {
		auto unit = wptr.second.lock();

		if(!unit) continue;

		unit->cycle_line(type);
	}
}

void Rack::sync(Profile & profile, SyncFlag flags) {

}
