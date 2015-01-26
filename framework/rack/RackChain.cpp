#include "RackChain.h"
using namespace RackoonIO;

void RackChain::addUnit(RackUnit *unit) {
	chain.push_back(unit);
	unit->setChain(this);
}


RackUnit *RackChain::getUnit(std::string name) {
	int sz = chain.size();
	for(int i = 0; i < sz; i++) {
		if(chain[i]->getName() == name)
			return chain[i];
	}

	return NULL;
}
