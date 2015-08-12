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
#include "framework/common.h"
#include "framework/factories/GenericEventMessageFactory.h"
#include <atomic>
#include <condition_variable>

namespace StrangeIO {

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
class EventLoop {
	/** An array of events, each holding an array of callbacks which go back to the listening objects */
	std::map< EventType, std::vector< std::function< void(std::shared_ptr<EventMessage>) > > > eventListeners;

	/** The queue of event messages waiting to be distributed to listeners */
	std::vector< std::unique_ptr<EventMessage> > eventQueue;
	int maxEventTypes; ///< The number of event message types

	std::mutex evLock; ///< The mutex for accessing the event queue
	std::atomic<bool> mData; ///< Flag to signify that data is waiting for processing
	std::atomic<bool> mRunning; ///< Flag to signify running state of loop
	std::atomic<bool> mActive; ///< Flag to signify whether the thread is active or not
	std::condition_variable cv;


	std::thread mLoopThread; ///< The thread for the event loop

	/** Internal method for distributing the message of an event
	 *
	 * When an event occurs, the EventMessage is passed to the loop's
	 * queue. When the EventLoop cycles, every message is passed to
	 * this method to be distributed listeners (if any).
	 *
	 * @param msg A unique_ptr to an event message.
	 */
	void distributeMessage(std::unique_ptr<EventMessage>);

	/** setup the framework's list of events
	 *
	 * The framework event IDs start at 1000 which
	 * means clients can use IDs from 0 - 999
	 */
	void frameworkInit();

	/** Check whether thread should continue waiting */
	bool unblock();
public:
	EventLoop();

	/** Assign an event listener to an event type
	 *
	 * Clients pass a callback function to be called when the event occurs
	 * in the loop. The EventType specifies what event will trigger callback.
	 *
	 * When the callback is used, the EventMessage is passed into as a paremeter
	 *
	 * @note
	 * The signature of a callback is:<br /><br />
	 * std::function<void(shared_ptr<EventMessage>
	 *
	 * @param callback The callback to the listening object
	 */
	void addEventListener(EventType, std::function<void(std::shared_ptr<EventMessage>)>);

	/** Send an EventMessage into the loop to be distributed to the listeners
	 *
	 * This method is called if an event has occurred and needs to be distributed
	 * out to the objects that have interest in the event. The EventMessage is any
	 * derived class, cast to EventMessage, to be recast on receipt of listener.
	 *
	 * An event message ownership is transferred to the event handler via a unique_ptr
	 *
	 * @param msg a unique_ptr to the EventMessage to be passed into the loop
	 */
	void addEvent(std::unique_ptr<EventMessage>);

	/** The event loop cycling thread
	 *
	 * This method is run in the event loop thread and handles
	 * the dispatch of event messages
	 */
	void cycle();

	/** Set the number of events that are used within the framework
	 *
	 * A client system defines their own list of events to be used by
	 * the processing units
	 *
	 * @note The client's event IDs can be from 0-999
	 *
	 * @param numEvents The number of events used in the system
	 */
	void initEvents(short);

	/** Start the event loop thread */
	void start();

	/** Stop the event loop */
	void stop();

	/** Check if the loop is running */
	bool isRunning();

	/** Check if the thread is active */
	bool isActive();
};
}
#endif
