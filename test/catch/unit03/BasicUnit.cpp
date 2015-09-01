#include "BasicUnit.hpp"
using namespace StrangeIO;
using namespace StrangeIO::component;

BasicUnit::BasicUnit(std::string label) :
Unit(UnitType::Mainliner, "Basic", label) {
}

BasicUnit::~BasicUnit() {
}

CycleState BasicUnit::cycle() {
	return CycleState::Complete;
}

CycleState BasicUnit::init() {
	return CycleState::Complete;
}

void BasicUnit::feed_line(Memory::CachePtr samples, int line) {
}

void BasicUnit::set_configuration(std::string, std::string) {
	
}

UnitBuilder(BasicUnit);
