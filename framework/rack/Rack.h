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
#ifndef RACK_H
#define RACK_H
#include <atomic>

#include "RackChain.h"
#include "framework/threads/RackQueue.h"
#include "RackConfig.h"
#include "framework/factories/RackUnitGenericFactory.h"
#include "framework/picojson/picojson.h"
#include "framework/midi/MidiHandler.h"
#include "framework/events/EventLoop.h"


namespace StrangeIO {
/** The virtual rack - the hub of the framework
 *
 * The rack is where RackUnit objects are plugged into and
 * cycled. It also cycles some of the other subsystems, like
 * the event loop and MIDI.
 */

/** @todo
 * This class is getting pretty close to becoming a demi-god and needs to be split
 */
class Rack {

	RackConfig rackConfig; ///< The configuration structure for the Rack
	RackState rackState; ///< Current Rack state
	RackQueue *mRackQueue; ///< The queue for worker thread packages

	std::vector<Plug*> plugArray; ///< The array of mainline plugs
	RackChain rackChain; ///< The daisychains of units @todo this class is probably unnecessary

	MidiHandler midiHandler; ///< The midi device and bindings manager
	EventLoop eventLoop; ///< The system event loop

	std::string configPath; ///< The path to the congfiguration file
	std::thread *mCycleThread;
	std::condition_variable mCycleCondition;
	std::atomic<int> mCycleCount;

	// config and init

	/** Internal function for loading and starting the parse of the configuration file */
	std::string loadConfig();

	/** Internal function parsing the configuration file
	 *
	 * This method is recursive and parses different braunches
	 * of the configuration
	 *
	 * @param v The JSON value to be parsed
	 * @param area The area of the configuration being parsed
	 */
	void parseConfig(picojson::value v, RConfigArea area);

	/** Parse the Rack setup (mainlines, daisychains and unit configs)
	 *
	 * This is where the unit configs are sent for parsing, and the
	 * instantiated units are connected together in daisychains and
	 * the top units are connected to the mainlines.
	 *
	 * @param v The JSON value containing the setup branches
	 */
	void parseRack(picojson::value);

	/** Parse the RackUnit configuration
	 *
	 * This method instantiates the RackUnit and parses
	 * the MIDI bindings
	 *
	 * @param name The unit's unique name
	 * @param v The JSON branch that has the configuration
	 */
	std::shared_ptr<RackUnit> parseUnit(std::string name, picojson::value);

	/** Parse the configured bindings and link them to the exported methods
	 *
	 * @param unit The RackUnit to parse for
	 * @param v The JSON branch with the bindings
	 */
	void parseBindings(std::shared_ptr<RackUnit>, picojson::value);

	/** Setup a default configuration */
	void initialConfig();

	/** Initialise the RackQueue and it's worker threads */
	void initRackQueue();

	/** The client supplied RackUnit factory */
	std::unique_ptr<RackUnitGenericFactory> unitFactory;

	/** Method callback when there is an event that will trigger a rack cycle */
	void onCycleEvent(std::shared_ptr<EventMessage>);

#if RACK_METRICS
	/** Callback for start of unit cycle - RACK_METRICS telemetry */
	std::function<void(std::chrono::steady_clock::time_point)> metricUnitCycleStart;

	/** Callback for end of unit cycle - RACK_METRICS telemetry */
	std::function<void(std::chrono::steady_clock::time_point)> metricUnitCycleEnd;
#endif

protected:
	//** The main system cycle - this is called when a rack cycle event is triggered */
	void cycle();

	/** Get a mainline plug from it's name
	 *
	 * @param name The name of the plug
	 */
	Plug *getPlug(std::string name) const;
public:
	/** Instantiate the rack in an RACK_OFF state */
	Rack();

	/** Change the target path of the configuration
	 *
	 * @param path Relative or absolute path to resource
	 */
	void setConfigPath(std::string);

	/** Initialise the rack with default values and parse the configuration */
	void init();

	/** Start the rack cycling - this also fires up the warm up cycle */
	void start();

	/** Set the number of events used within the client system
	 *
	 * @param num The number of events
	 */
	void initEvents(int num);

	/** Supply a pointer to client unit factory.
	 *
	 * The Rack takes ownership  of the pointer
	 *
	 * @param factory A unique_ptr to the client factory
	 */
	void setRackUnitFactory(std::unique_ptr<RackUnitGenericFactory> factory);

	/** Get an instantiated unit from the Rack
	 *
	 * @param name The unique name of the Unit
	 * @return A shared pointer to the unit
	 */
	std::shared_ptr<RackUnit> getUnit(std::string name);

	/** Get a a full list of units in the rack
	 *
	 * @return A map with shared pointers to units, keyed by their unique name
	 */
	std::map<std::string, std::shared_ptr<RackUnit>> getUnits();

	/** Retrieve the event loop from the Rack
	 *
	 * @return A pointer to the event loop
	 */
	EventLoop *getEventLoop();


#if RACK_METRICS
	/** Method for setting the callback for UnitCyle metrics - RACK_UNIT telemetry
	 *
	 * @param start The callback for registering the start of a cycle
	 * @param end The callback for registering the end of the cycle
	 */
	void cbmetricUnitCycle( std::function<void(std::chrono::steady_clock::time_point)> start, std::function<void(std::chrono::steady_clock::time_point)> end);
#endif

};


}

#ifdef RACK_METRICS
	#include "framework/telemetry/RackTelemetry.h"
#endif

#endif
