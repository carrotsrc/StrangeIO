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

/** The daisychains of units in the Rack
 *
 * @note
 * This class is probably unnecessary in its
 * current guise - the focus should either be
 * changed or it should be factored out
 */
class RackChain
{
	vector<RackUnit*> chain; ///< The vector of units in the daisychains

	int sampleRate; ///< The sample rate across the chain - pretty much unused
public:
	/** Register a unit in the daisychains
	 *
	 * @param unit A pointer to the RackUnit
	 */
	void addUnit(RackUnit *unit);

	/** Set the rack queue object to be injected into units
	 *
	 * When a unit is registered with the chain, some dependencies are
	 * injected.
	 *
	 * @todo: The dependencies probably should be handled by the factory
	 *
	 * @param queue A pointer to the RackQueue
	 */
	void setRackQueue(RackQueue *queue);

	//** @deprecated */
	void join();

	/** Get a unit from the chain
	 *
	 * @param name The unique name of the unit
	 * @return Pointer to the RackUnit; nullptr if unit doesn't exist
	 */
	RackUnit *getUnit(std::string name);

	/** Get a map of all the units in the chain
	 *
	 * @return a map of pointers to the RackUnit, keyed by their unique name
	 */
	std::map<std::string, RackUnit*> getUnits();
};
}
#endif
