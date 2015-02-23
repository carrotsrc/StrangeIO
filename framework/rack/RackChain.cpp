/* Copyright 2015 Charlie Fyvie-Gauld
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published 
 *  by the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
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
