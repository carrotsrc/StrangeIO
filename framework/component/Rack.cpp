#include <chrono>
#include "framework/component/Rack.hpp"
using namespace StrangeIO::Component;

using pclock = std::chrono::steady_clock;

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


CycleState Rack::cycle(CycleType type) {

	auto state = CycleState::Empty;

	for( auto& wptr : m_mainlines ) {
		auto unit = wptr.second.lock();
		if(!unit) continue;
		state = unit->cycle_line(type);
	}

	return state;
}

void Rack::sync(SyncFlag flags) {
	pclock::time_point t_start, t_end;

	if((flags & (SyncFlag)SyncFlags::SyncDuration)) {
		t_start = pclock::now();
	}

	cycle(CycleType::Sync);

	if((flags & (SyncFlag)SyncFlags::SyncDuration)) {
		t_end = pclock::now();
		m_rack_profile.sync_duration = std::chrono::duration_cast<ProfileDuration>(t_end-t_start);
	}

}

const RackProfile & Rack::rack_profile() {
	return m_rack_profile;
}
