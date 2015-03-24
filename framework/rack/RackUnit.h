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
#ifndef RACKUNIT_H
#define RACKUNIT_H
#include "UnitConnectors.h"
#include "framework/threads/RackQueue.h"
#include "framework/events/EventLoop.h"
#include "framework/memory/CacheHandler.h"
#include "framework/factories/GenericEventMessageFactory.h"
#include "framework/events/FrameworkMessages.h"

namespace RackoonIO {


/** Macro for simplifying the method exports for bindind to MIDI controllers
 *
 * @param action The string name of the action for binding
 * @param method The function to bind to the action
 */
#define MIDI_BIND(action, method) (midiExportMethod(action, std::bind(&method, this, std::placeholders::_1)))

/** Macro for adding an event listener
 *
 * @param type The type of event to listen for
 * @param callback The callback for when the event occurs
 */
#define EVENT_LISTENER(type, callback) (addEventListener(type, std::bind(&callback, this, std::placeholders::_1)))

/** Macro for outsourcing a task to worker threads
 *
 * @param task The method used for processing the task
 */
#define OUTSRC(task) (outsource(std::bind(&task, this)))

/** Macro for printing a unit message to the console
 *
 * @param type The unit class type
 * @param msg The message to print
 */
#define CONSOLE_MSG(type, msg) (cout << type << " [" << getName() << "]: " << msg << endl)
class RackChain;

/** Different activity states the unit can take on */
enum UnitState {
	UNIT_ACTIVE, ///< The unit is online and processing/ready to process
	UNIT_OFF ///< The unit is off
};


/** The base class for a processing unit in the Rack
 *
 * This is where all the processing action happens. A RackUnit
 * is an abstraction of a unit of audio processing hardware on
 * a rack.
 *
 * Each RackUnit is placed in a daisychain and performs a specific 
 * task like loading PCM samples from a file, applying a single
 * effect, mixing.
 *
 * It can be assumed that protected methods are client facing API
 * for the derived classes.
 *
 * When the RackUnit receives samples through it's feed method, it can either
 * accept the samples by returning FEED_OK, or tell the upstream unit to
 * hold on until the current processing has finished or is ready to accept
 * more data by returning FEED_WAIT.
 *
 * Once the data is accepted (by flushing the jack), the RackUnit can either 
 * outsource the task to a worker thread or perform the processing in the feed 
 * method.
 *
 * If the downstream can't accept the processed data, the unit must store
 * the data and wait for another opportunity, either from an AC signal or
 * another upstream feed. The state machine can be a bit of a headache to 
 * work out.
 *
 * The RackUnit can use the buffers provided or build its own.
 *
 * @todo This also is soon to become a demi-god and functionality needs breaking up
 */
class RackUnit
{
	vector<Jack*> jackArray; ///< A vector of Jack objects
	vector<Plug*> plugArray; ///< A vector Plugs objects

	std::string name; ///< The unique name of the RackUnit
	std::string rutype; ///< The string type name (e.g. the class name)

	RackQueue *rackQueue; ///< A pointer to the rackQueue
	GenericEventMessageFactory *messageFactory; ///< A pointer to the client supplied message factory
	EventLoop *eventLoop; ///< A pointer to the event loop
	CacheHandler *cacheHandler; ///< A pointer to the cache allocation handler

	std::map<string, std::function<void(int)> > midiExport; ///< A map of exported methods to bind to MIDI


protected:
	RackChain *chain; ///< Pointer to the whole daisychain
	UnitState unitState; ///< Current system state of the unit

	/** Add a jack to the unit
	 *
	 * This is used by derived classes to describe the array of jacks
	 * to be used for connections coming from upstream
	 * 
	 * @note
	 * The ConnectorType is *almost* deprecated unless a real use is
	 * found
	 *
	 * @param name The name given to the jack
	 * @param type The type of jack it is
	 */
	void addJack(string name, ConnectorType type);

	/** Add a plug to the unit
	 *
	 * This is used by derived classes to describe the array of plugs 
	 * that can be used to connect downstream
	 * 
	 * @param name The name given to the plug
	 */
	void addPlug(string name);

	void feedOut(string, short*); ///< @deprecated Unless use is found

	/** Outsource a task to the thread pool
	 *
	 * Call this method when a task is required to be run on a separate
	 * thread.
	 *
	 * This means derived classes don't have direct access to the queue
	 * object
	 *
	 * @param task The task method to outsource to the thread pool
	 */
	void outsource(std::function<void()> task);

	/** Export a method for MIDI binding
	 *
	 * Call this method to export a particular method in the derived
	 * class binding to a MIDI controller. The method will act as the 
	 * callback for when the controller code is received
	 *
	 * @param action The action name that can be configured for binding
	 * @param method The method callback to perform the action
	 */
	void midiExportMethod(string, std::function<void(int)> );

	/** Add an event listener to be notified of a particular event
	 *
	 * @param eventType The event type for which to be notified
	 * @param callback The callback method in which to recieve the notification
	 */
	void addEventListener(EventType eventType, std::function<void(shared_ptr<EventMessage>)> callback);

	/** Add an event to the event loop
	 *
	 * Call this method when an event has occurred within the system
	 * 
	 * @param msg The event message to be distributed
	 */
	void addEvent(std::unique_ptr<EventMessage> msg);

	/** Create a new message based on an event
	 *
	 * This will contruct an event message from the supplied factory.
	 *
	 * The message will automatically go through a generalised upcasting
	 * to EventMessage so it can be distributed throughout the event
	 * layer.
	 *
	 * @param eventType the event type for the message
	 * @return A unique_ptr to the instantiated message, upcast to EventMessage
	 */
	unique_ptr<EventMessage> createMessage(EventType eventType);

	/** Allocate a new block from the cache
	 *
	 * Instead of doing direct memory allocation - RackUnit objects are
	 * encouraged to use whatever CacheHandler is supplied. This is done
	 * via this API method.
	 *
	 * @param num The number of blocks to allocate
	 * @return A pointer to the allocated cache block
	 */
	short *cacheAlloc(int num);

	/** Free a block that has been allocated from the cache
	 *
	 * Instead of doing direct memory allocation - RackUnit objects are
	 * encouraged to use whatever CacheHandler is supplied. This is down
	 * via this API method.
	 *
	 * This method will allow the block to be reused in the system
	 *
	 * @param mem The pointer to the cache block
	 */
	void cacheFree(short *mem);

	/** Notify the system that processing is complete
	 *
	 * This will trigger the event FwProcComplete which, among other
	 * potential listeners, will cycle the rack. It is essentially
	 * a quick wrapper for addEvent(createMessage(FwProcComplete))
	 */
	void notifyProcComplete();
public:

	/** Sets unit to INACTIVE and sets the supplied unit type */
	RackUnit(string utype);

	/** Set the unit's unique name
	 *
	 * @param name The unique string name of the unit (probably set in configuration)
	 */
	void setName(string name);

	/** get the unit's unique name
	 *
	 * @return The string name of the unit
	 */
	string getName() const;

	/** Get the string Unit type 
	 *
	 * @return The string unit type
	 */
	std::string getRuType() const;

	/** Set the parent daisychain object that holds the unit 
	 *
	 * @note This is probably set by the parent chain itself
	 *
	 * @param chain A pointer to the chain
	 */
	void setChain(RackChain *chain);

	/** Get the parent chain
	 *
	 * @return A pointer to the parent chain
	 */
	RackChain *getChain() const;

	/** Set the pointer to the task queue
	 *
	 * @param queue The pointer to the queue
	 */
	void setRackQueue(RackQueue *queue);

	/** Set the pointer to the supplied message factory
	 *
	 * The message factory is upcast to the base class so it
	 * can be used for API
	 *
	 * @param queue The pointer to the queue
	 */
	void setMessageFactory(GenericEventMessageFactory*);

	/** Set the pointer to the supplied CacheHandler
	 *
	 * The handler is upcast to the base class so it
	 * can be used for API
	 *
	 * @param handler The pointer to the handler
	 */
	void setCacheHandler(CacheHandler *handler);

	/** Set the pointer to rack's event loop
	 *
	 * @param loop The pointer to the EventLoop
	 */
	void setEventLoop(EventLoop *loop);

	/** Get the jack of specified name
	 * 
	 * @param name The name of the jack
	 * @return Pointer to the Jack if it exists; otherwise nullptr
	 */
	Jack *getJack (std::string name) const;

	/** Get the plug of specified name
	 * 
	 * @param name The name of the plug
	 * @return Pointer to the Plug if it exists; otherwise nullptr
	 */
	Plug *getPlug (std::string name) const;

	/** Create a connection between two units
	 * 
	 * @param plug The name of the plug to connect from
	 * @param jack The name of the jack to connect to
	 * @param unit A pointer for unit to connect to
	 */
	void setConnection(string plug, string jack, RackUnit *unit);


	void join(); ///< @deprecated This hasn't found a real use yet
	void unjoin(); ///< @deprecated This hasn't found a real use yet

	/** feed method used by the Rack to singal AC or a state chaing
	 *
	 * This method is used in the Rack object's cycle to push signals
	 * down the daisychain. The point of contact are units on
	 * the mainlines.
	 *
	 * @param rackState The signal to push down the daisychain
	 * @return Any state changes in the daisychain itself.
	 */
	RackState rackFeed(RackState signal);

	/** Check whether the unit has exported methods for binding to MIDI
	 *
	 * @return true if the unit has bindable actions; false if there are not methods exported
	 */
	bool midiControllable();

	/** The map of actions and respective methods which have been exported for MIDI binding
	 *
	 * @return A map of actions and methods
	 */
	virtual std::map<string, std::function<void(int)> > midiExportedMethods();

	/** Set a specific configuration in the unit
	 *
	 * This is usually supplied by the configuration file
	 * 
	 * @param config The string label of the config
	 * @param value The string value to apply to the configuration
	 */
	virtual void setConfig(string config, string value) = 0;

	/** The method to signify and start processing samples
	 *
	 * This is a virtual method defined by derived classes. When
	 * sample data is ready to be fed into the unit (from upstream units)
	 * the Jack which is being fed the data calls this method to signify
	 * the unit that a sample period is waiting to be processed. The unit
	 * then uses this method to flush the jack and begin processing or
	 * tell the upstream to wait
	 *
	 * @param jack The pointer to the jack being fed data
	 * @return FEED_OK when data is accepted; otherwise FEED_WAIT
	 */
	virtual FeedState feed(Jack *jack) = 0;

	/** A method that is called when an AC signal is received
	 *
	 * This method is derived by child classes. This method
	 * is called whenever an AC signal is received by the RackUnit
	 * and is most likely every cycle.
	 *
	 * This can be used as an opportunity to trying feeding data that
	 * is waiting to go to the next unit and/or continue processing
	 *
	 * If needed, signal a state change to the rack unit
	 *
	 * @return RACK_UNIT_OK; otherwise any state changes
	 */
	virtual RackState cycle() = 0;


	/** Method that is called on the warm up cycle
	 *
	 * This method is defined by derived classes and is used
	 * to initialise the unit on the first warm up cycle.
	 * 
	 * Everything to get the unit up and running should
	 * happen here and return successfully or error out
	 *
	 * @retryn RACK_UNIT_OK; otherwise error out
	 */
	virtual RackState init() = 0;

	/** Ad hoc method for dealing with upstream blocks
	 *
	 * This method is defined to handle an event where
	 * there is a block upstream. The unit then deals
	 * with it and passes the block downstream if it 
	 * needs to. (A mixer, for instance, may continue
	 * processing the other channel rather than propoagating
	 * the block down stream).
	 *
	 * @todo
	 * This really neads to be integrated in a cleaner
	 * way.
	 *
	 * @param jack The jack that received the block signal
	 */
	virtual void block(Jack* jack) = 0;

};

}
#endif
