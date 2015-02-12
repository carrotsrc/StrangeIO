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

	return nullptr;
}

void RackChain::setRackQueue(RackQueue *queue) {
	int sz = chain.size();
	for(int i = 0; i < sz; i++)
		chain[i]->setRackQueue(queue);
}

std::map<std::string, RackUnit*> RackChain::getUnits() {
	std::map<std::string, RackUnit*> listing;
	std::vector<RackUnit*>::iterator it;
	for(it = chain.begin(); it != chain.end(); ++it)
		listing.insert( std::pair<std::string, RackUnit*>((*it)->getName(), *it));

	return listing;

}
