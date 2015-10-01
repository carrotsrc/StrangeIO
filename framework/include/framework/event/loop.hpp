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
#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include <atomic>
#include <map>
#include <vector>
#include <condition_variable>
#include <thread>

#include "framework/fwcommon.hpp"

#include "framework/event/event.hpp"
#include "framework/event/msg.hpp"
#include "framework/event/event_list.hpp"
#include "framework/event/loop_utility.hpp"

#include "framework/thread/task_utility.hpp"



namespace strangeio {
namespace event {

using callback_vec = std::vector<event_callback>;
using listener_map = std::map<event_type, callback_vec>;
using event_queue = std::vector<msg_uptr>;

/** StrangeIO's inbuilt event loop
 *
 * The main idea is to have an map of events. Objects
 * can add event listeners and send out event messages.
 *
 * When an event message is sent to the loop, it
 * distributes the message to all the listeners.
 *
 * Event messages are derived classes of type EventMessage
 * and are handled as EventMessage types until they reach a
 * location that knows how to cast it to the original message.
 * Generally an event listener will know what to do with the
 * message.
 *
 */

class loop 
: public loop_utility
, public thread::task_utility {
	/** An array of events, each holding an array of callbacks which go back to the listening objects */
	
	listener_map m_listeners;

	/** The queue of event messages waiting to be distributed to listeners */
	event_queue m_queue;

	int m_max_types; ///< The number of event message types

	std::mutex m_tail_mutex; ///< The mutex dealing with the tail contention

	event_list *m_head, 
		   *m_tail;

	std::atomic_uintptr_t m_tail_ptr;

#if DEVBUILD
	std::atomic_uint m_load;
	unsigned int m_task_queue;
	unsigned int m_max_queue;
#endif

	/**
	 * @brief The event queue cycle task
	 * 
	 * The method is sent as a task to the work
	 * threads.
	 */
	void cycle_events();

public:
	loop();

	/** Assign an event listener to an event type
	 *
	 * Clients pass a callback function to be called when the event occurs
	 * in the loop. The EventType specifies what event will trigger callback.
	 *
	 * When the callback is used, the EventMessage is passed into as a paremeter
	 *
	 * @note
	 * The signature of a callback is:<br /><br />
	 * std::function<void(shared_ptr<EventMessage>)>
	 *
	 * @param callback The callback to the listening object
	 */
	void add_listener(event_type type, event_callback callback);

	/** Send an EventMessage into the loop to be distributed to the listeners
	 *
	 * This method is called if an event has occurred and needs to be distributed
	 * out to the objects that have interest in the event. The EventMessage is any
	 * derived class, cast to EventMessage, to be recast on receipt of listener.
	 *
	 * An event message ownership is transferred to the event handler via a unique_ptr
	 *
	 * @param msg a unique_ptr to the msg to be passed into the loop
	 */
	void add_event(msg_uptr message);

	unsigned int size();
	unsigned int listeners(event_type type);

#if DEVBUILD
	unsigned int load();
	unsigned int max_queue();
#endif
};

} // event
} // strangeio
#endif
