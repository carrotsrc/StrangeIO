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
#include <functional>


#include "UnitConnectors.h"
#include "framework/threads/RackQueue.h"
#include "framework/events/EventLoop.h"
#include "framework/memory/CacheHandler.h"
#include "framework/factories/GenericEventMessageFactory.h"

namespace RackoonIO {
#define MIDI_BIND(name, func) (midiExportMethod(name, std::bind(&func, this, std::placeholders::_1)))
#define EVENT_LISTENER(type, callback) (addEventListener(type, std::bind(&callback, this, std::placeholders::_1)))
#define OUTSRC(func) (outsource(std::bind(&func, this)))
#define CONSOLE_MSG(name, msg) (cout << name << " [" << getName() << "]: " << msg << endl)
class RackChain;
enum UnitState {
	UNIT_ACTIVE,
	UNIT_OFF
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
public:
	RackUnit(string);

	void setName(string);
	string getName() const;
	std::string getRuType() const;

	void setChain(RackChain*);
	RackChain *getChain() const;

	void setRackQueue(RackQueue*);
	void setMessageFactory(GenericEventMessageFactory*);
	void setCacheHandler(CacheHandler*);

	void setEventLoop(EventLoop*);

	Jack *getJack (string) const;
	Plug *getPlug (string) const;

	void setConnection(string, string, RackUnit*);

	void join();
	void unjoin();

	RackState rackFeed(RackState);

	bool midiControllable();
	virtual std::map<string, std::function<void(int)> > midiExportedMethods();

	virtual void setConfig(string, string) = 0;
	virtual FeedState feed(Jack*) = 0;

	virtual RackState cycle() = 0;
	virtual RackState init() = 0;

	virtual void block(Jack*) = 0;

};

}
#endif
