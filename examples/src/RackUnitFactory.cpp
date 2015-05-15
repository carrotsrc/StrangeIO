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
#include "RackUnitFactory.h"
#include "RuAlsa.h"
#include "RuImpulse.h"

using namespace RackoonIO;
using namespace ExampleCode;

std::unique_ptr<RackUnit> RackUnitFactory::build(std::string type, std::string name) {
	RackUnit *unit = NULL;
	if(type == "Alsa")
		unit = (RackUnit*) new RuAlsa();
	else
	if(type == "Impulse")
	unit = (RackUnit*) new RuImpulse();
	else
		return nullptr;

	unit->setName(name);
	setDependencies(unit);
	return std::unique_ptr<RackUnit>(unit);
}
