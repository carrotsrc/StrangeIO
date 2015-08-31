#include "BasicUnit.hpp"
using namespace StrangeIO::Component;

BasicUnit::BasicUnit(std::string label) :
Unit(UnitType::Mainliner, "Basic", label) {
}

BasicUnit::~BasicUnit() {
}

CycleState BasicUnit::cycle() {
}

CycleState BasicUnit::init() {
}

void BasicUnit::feed_line(Memory::CachePtr samples, int line) {
}

void BasicUnit::set_configuration(std::string, std::string) {
	
}
