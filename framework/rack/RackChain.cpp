#include "RackChain.h"
using namespace RackoonIO;

void RackChain::addUnit(RackUnit *unit) {
	chain.push_back(unit);
	unit->setChain(this);
}
