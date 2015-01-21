#include "RackChain.h"

void RackChain::addUnit(RackUnit *unit) {
	chain.push_back(unit);
	unit->setChain(this);
}
