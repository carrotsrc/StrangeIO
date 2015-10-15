#include <chrono>

#include "framework/fwcommon.hpp"
#include "framework/component/backend.hpp"

using namespace strangeio::component;
using pclock = std::chrono::steady_clock;

backend::backend()
	: m_global_profile({0})
{
	m_rack_profile = {
		.sync_duration = profile_duration::zero(),
		.cycle_duration = profile_duration::zero(),
	};
}

backend::~backend() {
}

cycle_state backend::cycle(cycle_type type) {
	auto state = cycle_state::empty;
	for( auto uptr : m_raw_mainlines ) {
		state = uptr->cycle_line(type);
	}

	return state;
}

void backend::sync(sync_flag flags) {
	//std::cout << "[Resycning]" << std::endl;
	if((flags & (sync_flag)sync_flags::sync_duration)) {
		return profile_sync(flags);
	}

	if((flags & (sync_flag) sync_flags::glob_sync)) {

		sync(m_global_profile, (sync_flag)sync_flags::none);
		sync(m_global_profile, flags);
		resync();

	} else if(flags & (sync_flag) sync_flags::upstream) {

		for(auto uptr : m_raw_mainlines) {
			sync_profile bogus = global_profile();
			uptr->sync_line(bogus, flags, 0);
		}

	}

	cycle(cycle_type::sync);

}

void backend::sync(sync_profile& profile, sync_flag flags) {
	for(auto uptr : m_raw_mainlines) {
			uptr->sync_line(profile, flags, 0);
	}
}

void backend::profile_sync(sync_flag flags) {
	pclock::time_point t_start, t_end;

	t_start = pclock::now();
	cycle(cycle_type::sync);
	t_end = pclock::now();

	m_rack_profile.sync_duration = std::chrono::duration_cast<profile_duration>(t_end-t_start);
}

bool backend::profile_line(sync_profile& profile, std::string mainline) {
	auto it = m_mainlines.find(mainline);

	if(it == m_mainlines.end()) return false;
	
	it->second->sync_line(profile, 0, 0);
	return true;
}

const backend_profile & backend::profile() {
	return m_rack_profile;
}

const sync_profile & backend::global_profile() {
	return m_global_profile;
}

