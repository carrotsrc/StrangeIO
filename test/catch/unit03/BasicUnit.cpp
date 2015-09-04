#include "BasicUnit.hpp"
using namespace strangeio;
using namespace strangeio::component;

BasicUnit::BasicUnit(std::string label) :
unit(unit_type::mainline, "Basic", label) {
}

BasicUnit::~BasicUnit() {
}

cycle_state BasicUnit::cycle() {
	return cycle_state::complete;
}

cycle_state BasicUnit::init() {
	return cycle_state::complete;
}

void BasicUnit::feed_line(memory::cache_ptr samples, int line) {
}


void BasicUnit::set_configuration(std::string, std::string) { }

UnitBuilder(BasicUnit);
