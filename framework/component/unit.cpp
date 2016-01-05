#include "framework/component/unit.hpp"
#include <iostream>

using namespace strangeio::component;

unit::unit(unit_type utype, std::string umodel, std::string ulabel)
	: linkable()
	, memory::cptr_utility()
	, thread::task_utility()
	, event::event_utility()

	// Protected
	, m_unit_profile({0})
	, m_upstream(false)

	// Private
	, m_utype(utype)
	, m_umodel(umodel)
	, m_ulabel(ulabel)
	, m_cstate(component_state::inactive)
	, m_rack(nullptr)
	, m_line_profile({0})

	, m_global_profile({0})
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

void unit::set_rack(rack_utility *rack) {
	m_rack = rack;
}

void unit::log(std::string msg) {
	std::cout << umodel() << " [" << ulabel() << "]: ";
	std::cout << msg;
	std::cout << std::endl;
}

void unit::set_configuration(std::string key, std::string value) { }

#if DEVBUILD
std::string unit::get_configuration(std::string key) {
	return std::string();
}
#endif
void unit::register_metric(profile_metric type, int value) {
	switch(type) {

	case profile_metric::latency:
		m_unit_profile.latency = value;
		break;

	case profile_metric::channels:
		m_unit_profile.channels = value;
		break;

	case profile_metric::period:
		m_unit_profile.period = value;
		break;

	case profile_metric::fs:
		m_unit_profile.fs = value;
		break;

	case profile_metric::drift:
		m_unit_profile.drift = (value/100.0f);
		break;

	case profile_metric::state:
		m_unit_profile.state = value;
		break;
		
	case profile_metric::bpm:
		m_unit_profile.bpm = value;
		
	case profile_metric::fill:
		m_unit_profile.fill = value;
		break;
	}
}

void unit::register_metric(profile_metric type, float value) {
	switch(type) {

	case profile_metric::latency:
		m_unit_profile.latency = value;
		break;

	case profile_metric::channels:
		m_unit_profile.channels = value;
		break;

	case profile_metric::period:
		m_unit_profile.period = value;
		break;

	case profile_metric::fs:
		m_unit_profile.fs = value;
		break;

	case profile_metric::drift:
		m_unit_profile.drift = value;
		break;

	case profile_metric::state:
		m_unit_profile.state = value;
		break;
		
	case profile_metric::bpm:
		m_unit_profile.bpm = value;
		break;

	case profile_metric::fill:
		m_unit_profile.fill = value;
		break;
	}
}

const sync_profile& unit::line_profile() const {
	return m_line_profile;
}

const sync_profile& unit::unit_profile() const {
	return m_unit_profile;
}

const sync_profile& unit::global_profile() const {
	return m_global_profile;
}

cycle_state unit::cycle_line(cycle_type type) {
	auto state = cycle_state::complete;

	switch(type) {

	case cycle_type::ac:
		if(m_line_profile.state == (int)line_state::flushing)
			break;
		
		state = cycle();
		break;

	case cycle_type::sync:
		m_line_profile.state = (int) line_state::active;
		m_line_profile.period = 0;
		sync_line(m_line_profile, (sync_flag)sync_flags::source, 0);
		break;

	case cycle_type::warmup:
		if(m_cstate >  component_state::inactive) {
			break;
		} else if((state = init()) == cycle_state::complete) {
			m_cstate = component_state::active;
		}
		break;
	}

	if(state > cycle_state::complete) {
		return state;
	}

	for(auto& output : outputs()) {
		if(output.connected) {
			output.to->unit->cycle_line(type);
		}
	}
	return state;
}
void unit::sync_line(sync_profile & profile, sync_flag flags, unsigned int line) {

	if( (flags & (sync_flag)sync_flags::glob_sync) ) {
		apply_unit_profile(profile, flags);
		continue_sync(profile, flags);
		
		m_global_profile.fs = profile.fs;
		m_global_profile.channels = profile.channels;
		m_global_profile.period = profile.period;
		m_global_profile.state = profile.state;
		m_global_profile.bpm = profile.bpm;
		m_global_profile.fill = profile.fill;
		resync(flags);
		return;
	}

	if( flags & (sync_flag)sync_flags::upstream) {

		if(m_upstream) {
			/* this unit requested an upstream sync
			 * switch off flag and propogate this unit's
			 * line profile
			 */
			m_upstream = false;
			resync(flags);
			return continue_sync(m_unit_profile, flags);
		} else {
			// set to the current line state
			//register_metric(profile_metric::state, profile.state);
			return continue_sync(profile, flags);
		}
		
	}
	
	if( ! (flags & (sync_flag)sync_flags::source) ) {
		apply_unit_profile(profile, flags);
	} else {
		// Turn off the source flag
		flags ^= (sync_flag)sync_flags::source;
	}

	continue_sync(profile, flags);
	resync(flags);
}

void unit::continue_sync(sync_profile& profile, sync_flag flags) {

	for(const auto& out : outputs()) {
		if(out.connected == true && out.to->unit != nullptr) {
			out.to->unit->sync_line(profile, flags, out.to->id);
		}
	}
}

void unit::apply_unit_profile(sync_profile& profile, sync_flag flags) {
			// this is not the source of the line sync
		if(m_unit_profile.channels > 0)
			profile.channels = m_unit_profile.channels;

		if(m_unit_profile.period > 0 && profile.period < m_unit_profile.period) {
			profile.period = m_unit_profile.period;
		}

		if(m_unit_profile.fs > 0)
			profile.fs = m_unit_profile.fs;
		
		if(m_unit_profile.bpm > 0)
			profile.bpm = m_unit_profile.bpm;
		
		if(m_unit_profile.fill > 0 && profile.fill < m_unit_profile.fill)
			profile.fill = m_unit_profile.fill;

		if(m_unit_profile.state == (int)line_state::flushing)
			profile.state = m_unit_profile.state;
		
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
}

void unit::fill_line(memory::cache_ptr samples, int id) {
	for(const auto& out : outputs()) {
		if(out.connected == true && out.to->unit != nullptr) {
			out.to->unit->fill_line(samples, out.to->id);
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

void unit::register_midi_led(std::string state_name, int state) {
	m_leds.insert(
		std::pair<std::string, int>(state_name, state)
	);
}

const midi_handler_map& unit::midi_handlers() {
	return m_handlers;
}

const midi_led_map& unit::midi_leds() {
	return m_leds;
}

bool unit::controllable() {
	if(m_handlers.size() == 0) return false;
	return true;
}

void unit::trigger_cycle() {
	m_rack->trigger_cycle();
}

void unit::trigger_sync(sync_flag flags) {
	if(flags & (sync_flag) sync_flags::upstream)
		m_upstream = true;

	if(!m_rack) return;

	m_rack->trigger_sync(flags);
}

void unit::toggle_led(int state) {
	this->toggle_led_state(ulabel(), state);
}

cycle_state unit::resync(sync_flag flags) { return cycle_state::complete; }