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
#ifndef GENERICEVENTMESSAGEFACTORY_H
#define GENERICEVENTMESSAGEFACTORY_H

#include "common.h"
#include "framework/events/EventMessage.h"

namespace RackoonIO {
/** An abstract class for an EventMessage factory
 *
 * Since clients have their own list of events, the factory
 * is derived from this abstract class. This enables the framework
 * to inject the client defined factory into any processing units.
 *
 * The RackUnit defines a client facing method to generate messages
 * which is why the createMessage signature is declared.
 */

/** @todo
 *  Depending on the frequency of creation
 *  the event messages could be pulled out from a cache
 *  instead of being freshly allocated
 */
class GenericEventMessageFactory {
private:

	virtual std::unique_ptr<EventMessage> frameworkMessage(EventType)
protected:
	virtual std::unique_ptr<EventMessage> clientMessage(EventType)
	{return nullptr;};

public:
	/** The method to be defined by client Message factories
	 *
	 * This should return a freshly allocated EventMessage based
	 * on the respective EventType
	 *
	 * @param eventType the event type of message
	 * @return A unique_ptr of a new EventMessage
	 */
	std::unique_ptr<EventMessage> createMessage(EventType);
};

}
#endif
