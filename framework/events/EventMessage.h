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
#ifndef EVENTMESSAGE_H
#define EVENTMESSAGE_H

namespace RackoonIO {

/** The typename of an Event */
typedef short EventType;

/** The parent class to derive client event messages from
 *
 * This is used by the EventLoop as a general type
 * for any messages passed to it, associated with an
 * event. The clients will know what class to cast
 * the message into to get the payload details
 */
class EventMessage {
public:
	/** The destructor will zero the msgType */
	virtual ~EventMessage() { msgType = 0; };

	EventType msgType; ///< The event that is associated with the message
};

}
#endif
