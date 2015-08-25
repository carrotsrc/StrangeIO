#include "framework/component/Unit.hpp"
#include "framework/component/Rack.hpp"

using namespace StrangeIO::Component;

Unit::Unit(UnitType utype, std::string umodel, std::string ulabel) :
m_utype(utype), m_umodel(umodel), m_ulabel(ulabel), m_cstate(ComponentState::Inactive),
m_rack(nullptr)
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
	if(!m_rack) return;

	m_rack->register_metric(type,value);
}

void Unit::set_rack(Rack* ptr_rack) {
	m_rack = ptr_rack;
}

LineProfile& Unit::profile() {
	return m_profile;
}

CycleState Unit::run_cycle(CycleType type) {

	auto state = CycleState::Complete;
	switch(type) {
	case CycleType::Ac:
		state = cycle();
		break;

	case CycleType::Sync:
		sync_line(m_profile);
		break;

	case CycleType::Warmup:
		state = init();
		break;
	}

	if(state > CycleState::Complete) {
		return state;
	}

	return state;
}

CycleState Unit::cycle() {
	return CycleState::Complete;
}

CycleState Unit::init() {
	return CycleState::Complete;
}

void Unit::sync_line(LineProfile & profile) {
	return;
}
