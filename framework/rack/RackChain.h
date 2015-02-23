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
#ifndef RACKCHAIN_H
#define RACKCHAIN_H
#include "RackUnit.h"
namespace RackoonIO {

class RackChain
{
	vector<RackUnit*> chain;

	int sampleRate;
public:
	void addUnit(RackUnit*);
	void setRackQueue(RackQueue*);
	void join();

	RackUnit *getUnit(std::string);
	std::map<std::string, RackUnit*> getUnits();
};
}
#endif
