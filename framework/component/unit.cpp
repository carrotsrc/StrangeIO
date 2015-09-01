#include "framework/component/unit.hpp"
#include <iostream>

using namespace strangeio::component;

unit::unit(unit_type utype, std::string umodel, std::string ulabel) :
linkable(), m_utype(utype), m_umodel(umodel), m_ulabel(ulabel), m_cstate(component_state::Inactive),
m_rack(nullptr), m_line_profile({0}), m_unit_profile({0})
{ }

unit_type unit::utype() const {
	return m_utype;
}

std::string unit::umodel() const {
	return m_umodel;
}

std::string unit::ulabel() const {
	return m_ulabel;
}

component_state unit::cstate() const {
	return m_cstate;
}

void unit::change_cstate(component_state state) {
	m_cstate = state;
}

void unit::set_rack(RackUtilityInterface *rack) {
	m_rack = rack;
}

void unit::log(std::string msg) {
	std::cout << umodel() << " [" << ulabel() << "]: ";
	std::cout << msg;
	std::cout << std::endl;
}

void unit::register_metric(profile_metric type, int value) {
	switch(type) {

	case profile_metric::Latency:
		m_unit_profile.latency = value;
		break;

	case profile_metric::Channels:
		m_unit_profile.channels = value;
		break;

	case profile_metric::Period:
		m_unit_profile.period = value;
		break;

	case profile_metric::Fs:
		m_unit_profile.fs = value;
		break;

	case profile_metric::Drift:
		m_unit_profile.drift = (value/100.0f);
		break;

	}
}

void unit::register_metric(profile_metric type, float value) {
	switch(type) {

	case profile_metric::Latency:
		m_unit_profile.latency = value;
		break;

	case profile_metric::Channels:
		m_unit_profile.channels = value;
		break;

	case profile_metric::Period:
		m_unit_profile.period = value;
		break;

	case profile_metric::Fs:
		m_unit_profile.fs = value;
		break;

	case profile_metric::Drift:
		m_unit_profile.drift = value;
		break;

	}
}

const profile& unit::line_profile() const {
	return m_line_profile;
}

const profile& unit::unit_profile() const {
	return m_unit_profile;
}

cycle_state unit::cycle_line(cycle_type type) {

	auto state = cycle_state::Complete;
	switch(type) {

	case cycle_type::Ac:
		state = cycle();
		break;

	case cycle_type::Sync:
		sync_line(m_line_profile, (sync_flag)SyncFlags::Source);
		break;

	case cycle_type::Warmup:
		if(m_cstate >  component_state::Inactive) {
			break;
		} else if((state = init()) == cycle_state::Complete) {
			m_cstate = component_state::Active;
		}
		break;
	}

	if(state > cycle_state::Complete) {
		return state;
	}

	for(auto& output : outputs()) {
		if(output.connected) {
			output.to->unit->cycle_line(type);
		}
	}
	return state;
}
void unit::sync_line(profile & profile, sync_flag flags) {

	if( ! (flags & (sync_flag)SyncFlags::Source) ) {
		// this is not the source of the line sync
		if(m_unit_profile.channels > 0)
			profile.channels = m_unit_profile.channels;

		if(m_unit_profile.period > 0)
			profile.period = m_unit_profile.period;

		if(m_unit_profile.fs > 0)
			profile.fs = m_unit_profile.fs;

		profile.jumps++;

		// accumulate the drift percentage
		if( m_unit_profile.drift != 0.0f) {

			auto shift = m_unit_profile.drift;

			if(profile.drift > 0.0f) {
				shift = profile.drift + ( profile.drift * m_unit_profile.drift);
			} else if(profile.drift < 0.0f) {
				shift = profile.drift + ( (profile.drift*-1) * m_unit_profile.drift);
			}

			profile.drift = shift;
		}
		// accumulate latency in periods
		profile.latency += m_unit_profile.latency;


	} else {
		// Turn off the source flag
		flags |= (sync_flag)SyncFlags::Source;
	}

	for(const auto& out : outputs()) {
		if(out.connected == true && out.to->unit != nullptr) {
			out.to->unit->sync_line(profile, flags);
		}
	}
}

void unit::register_midi_handler(std::string binding_name, midi_method method) {
	m_handlers.insert(
		std::pair<std::string, midi_method>(
		binding_name, method
		)
	);
}

const midi_handler_map& unit::midi_handlers() {
	return m_handlers;
}
