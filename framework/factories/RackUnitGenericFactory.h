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
#ifndef RACKUNITGENERICFACTORY_H
#define RACKUNITGENERICFACTORY_H
#include "framework/rack/RackUnit.h"
#include "framework/hosting/PlatformHosting.h"
#include "framework/factories/GenericEventMessageFactory.h"
namespace RackoonIO {

/** The class from which to derive a client RackUnit factory
 *
 * Since the client defines their own sets of RackUnits, the
 * framework declares an abstract class to derive the client
 * factory.
 *
 * This abstract factory also injects important objects required
 * for the functioning of a RackUnit; when the client factory 
 * creates a new RackUnit, it must pass the unit object into
 * the passDependencies to initialise the object properly, before
 * passing it out to the caller.
 */
class RackUnitGenericFactory {
protected:
	EventLoop *eventLoop; ///< pointer to the framework's EventLoop
	GenericEventMessageFactory *messageFactory; ///< pointer to the client supplied Message factory
	CacheHandler *cacheHandler; ///< Pointer to the built in cache allocation handler
#if HOSTING_LV2
	Hosting::LV2Platform *lv2Platform;
#endif

	/** Internal method for injecting framework dependencies in new units
	 *
	 * New units should be passed in here before being passed out
	 * to the caller, in order that they initialised to function
	 * in the framework correctly.
	 */
	void setDependencies(RackUnit *unit);

public:
	/** The method called for building a unit
	 *
	 * This method must be defined by the client factory
	 *
	 * @param unitType The string name of the unit to construct
	 * @param unitName The unique string name of the unit
	 * @return unique_ptr to newly constructed unit; nullptr otherwise
	 */
	virtual std::unique_ptr<RackUnit> build(std::string, std::string) = 0;

	/** Supply an EventLoop for initialising new units 
	 * @param eventLoop Pointer to the framework's EventLoop
	 */
	void setEventLoop(EventLoop*);
	/** Supply an client message factory for initialising new units 
	 *
	 * @param messageFactory A pointer to the client supplied Message Factory
	 */
	void setMessageFactory(GenericEventMessageFactory*);
	/** Supply a CacheHandler for initialising new units
	 *
	 * @param cacheHandler The Cache Allocation handler that is built into the framework
	 */
	void setCacheHandler(CacheHandler*);

#if HOSTING_LV2
	void setLV2Platform(Hosting::LV2Platform*);
#endif

};

}
#endif
