#include "framework/component/Unit.hpp"
#include "framework/component/Rack.hpp"

using namespace StrangeIO::Component;

Unit::Unit(UnitType utype, std::string umodel, std::string ulabel) :
Linkable(), m_utype(utype), m_umodel(umodel), m_ulabel(ulabel), m_cstate(ComponentState::Inactive),
m_line_profile({0}), m_unit_profile({0})
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

void Unit::change_cstate(ComponentState state) {
	m_cstate = state;
}


void Unit::register_metric(ProfileMetric type, int value) {
	switch(type) {

	case ProfileMetric::Latency:
		m_unit_profile.latency = value;
		break;

	case ProfileMetric::Channels:
		m_unit_profile.channels = value;
		break;

	case ProfileMetric::Period:
		m_unit_profile.period = value;
		break;

	case ProfileMetric::Fs:
		m_unit_profile.fs = value;
		break;

	case ProfileMetric::Drift:
		m_unit_profile.drift = value;
		break;

	}
}

const Profile& Unit::line_profile() const {
	return m_line_profile;
}

CycleState Unit::cycle_line(CycleType type) {

	auto state = CycleState::Complete;
	switch(type) {

	case CycleType::Ac:
		state = cycle();
		break;

	case CycleType::Sync:
		sync_line(m_line_profile);
		break;

	case CycleType::Warmup:
		if(m_cstate >  ComponentState::Inactive) {
			break;
		} else if((state = init()) == CycleState::Complete) {
			m_cstate = ComponentState::Active;
		}
		break;
	}

	if(state > CycleState::Complete) {
		return state;
	}

	return state;
}


void Unit::sync_line(Profile & profile) {
	return;
}
