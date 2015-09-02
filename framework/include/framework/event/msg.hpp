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
#include <memory>

namespace strangeio {
namespace event {

/** The typename of an Event */
using event_type = short;

/** The parent class to derive client event messages from
 *
 * This is used by the lopp as a general type
 * for any messages passed to it, associated with an
 * event. The clients will know what class to cast
 * the message into to get the payload details
 */
class msg {
public:
	/** The destructor will zero the type */
	virtual ~msg() { type = 0; };

	event_type type; ///< The event that is associated with the message
};

using msg_uptr = std::unique_ptr<msg>;
using msg_sptr = std::shared_ptr<msg>;
using event_callback = std::function<void(msg_sptr)>;

} // event
} // strangeio
#endif
