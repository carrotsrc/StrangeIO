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
#include "RackChain.h"
#include "framework/threads/RackQueue.h"
#include "RackConfig.h"
#include "framework/factories/RackUnitGenericFactory.h"
#include "framework/picojson/picojson.h"
#include "MidiRouter.h"
#include "framework/events/EventLoop.h"

namespace RackoonIO {
class Rack {

	RackConfig rackConfig;
	RackState rackState;
	RackQueue *rackQueue;

	std::vector<Plug*> plugArray;
	RackChain rackChain;

	MidiRouter midiRouter;
	EventLoop eventLoop;

	std::chrono::microseconds uSleep;
	std::thread *cycleThread;
	std::string configPath;

	// config and init
	std::string loadConfig();
	void parseConfig(picojson::value, RConfigArea);
	void parseRack(picojson::value);
	RackUnit *parseUnit(std::string name, picojson::value);
	void parseBindings(RackUnit*, picojson::value);
	void initialConfig();
	void initRackQueue();

	std::unique_ptr<RackUnitGenericFactory> unitFactory;

#ifdef RACK_METRICS
	std::function<void(std::chrono::microseconds)> metricUnitCycleStart;
	std::function<void(std::chrono::microseconds)> metricUnitCycleEnd;
#endif

protected:
	void cycle();
	Plug *getPlug(string name) const;
public:
	Rack();

	void setConfigPath(std::string);
	void init();
	void initEvents(int);
	void start();

	void setRackUnitFactory(unique_ptr<RackUnitGenericFactory>);

	RackUnit *getUnit(std::string);
	std::map<std::string, RackUnit*> getUnits();

	EventLoop *getEventLoop();


#ifdef RACK_METRICS
	void cbmetricUnitCycle(
			std::function<void(std::chrono::microseconds)>, 
			std::function<void(std::chrono::microseconds)>
			);
#endif

};


}

#ifdef RACK_METRICS
	#include "framework/telemetry/RackTelemetry.h"
#endif

#endif 
