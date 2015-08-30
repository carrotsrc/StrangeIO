#include <chrono>
#include <iostream>
#include "framework/component/Rack.hpp"
using namespace StrangeIO::Component;

using pclock = std::chrono::steady_clock;

Rack::Rack() :
m_cache(nullptr), m_resync(false)
{ 
	m_rack_profile = {
		.sync_duration = ProfileDuration::zero(),
		.cycle_duration = ProfileDuration::zero(),
	};
}

Rack::~Rack() {

}

void Rack::set_cache_utility(Memory::CacheUtilityInterface* cache) {
	m_cache = cache;
}

void Rack::add_unit(unit_uptr unit) {
	unit->set_rack(this);
	unit->set_cache_utility(m_cache);
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

bool Rack::connect_mainline(std::string mainline, std::string unit) {

	auto line = m_mainlines.find(mainline);

	if(line == m_mainlines.end()) return false;

	auto wptr = get_unit(unit);
	if(wptr.expired()) return false;

	line->second = wptr;

#if DEVBUILD
	std::cout << "[Connection]\t" << 
		"rack." << mainline << 
		" --> " <<
		unit << ".power" << std::endl;
#endif

	return true;
}

bool Rack::connect_units(std::string from, std::string out, std::string to, std::string in) {
		auto ufrom = get_unit(from);
		if(ufrom.expired()) return false;


		auto uto = get_unit(to);
		if(uto.expired()) return false;

		auto from_shr = ufrom.lock();
		auto to_shr = uto.lock();

		auto output_id = from_shr->has_output(out);
		auto input_id = to_shr->has_input(in);

		if(output_id < 0 || input_id < 0) return false;


		auto link_in = const_cast<LinkIn*>(to_shr->get_input(input_id));
		auto ret = from_shr->connect(output_id, link_in);
#if DEVBUILD
		if(ret) {
			std::cout << "[Connection]\t" << 
				from << "." << out << 
				" --> " <<
				to << "." << in << std::endl;
		} else {
			std::cout << "[Connection failure]" << std::endl;
		}
#endif
		return ret;
}

void Rack::toggle_resync() {
	m_resync = true;
}

#include <iostream>
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

	if((flags & (SyncFlag)SyncFlags::SyncDuration)) {
		return profile_sync(flags);
	}

	cycle(CycleType::Sync);

}

void Rack::profile_sync(SyncFlag flags) {
		pclock::time_point t_start, t_end;

		t_start = pclock::now();
		cycle(CycleType::Sync);
		t_end = pclock::now();

		m_rack_profile.sync_duration = std::chrono::duration_cast<ProfileDuration>(t_end-t_start);
}

bool Rack::profile_line(Profile & profile, std::string mainline) {
	auto it = m_mainlines.find(mainline);

	if(it == m_mainlines.end()) return false;
	if(it->second.expired()) return false;
	auto shr = it->second.lock();
	shr->sync_line(profile, 0);
	return true;
}

const RackProfile & Rack::rack_profile() {
	return m_rack_profile;
}
