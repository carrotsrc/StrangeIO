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

/* QO directives for compilation
 *
 * This is experimental build process using qo
 * (http::https://github.com/andlabs/qo)
 *
 * with added output directive
 *
 *
// #qo pkg-config: --libs alsa
// #qo CXXFLAGS: -fPIC -shared -I../
// #qo LDFLAGS: -fPIC -shared -ggdb -Wl,-soname,librackio.so
// #qo output: librackio.so
 */
#include <fstream>
#include "Rack.h"
#include "framework/events/FrameworkMessages.h"

using namespace StrangeIO;
Rack::Rack() {
	rackState = RACK_OFF;
	configPath = "StrangeIO.cfg";
	mCycleCount = 0;
}
#define PICO picojson

void Rack::init() {
	initialConfig();
	std::string json = loadConfig();
	picojson::value v;
	std::string err = picojson::parse(v, json);
	if(err.empty())
		parseConfig(v, ROOT);
	else
		std::cout << err << std::endl;
	initRackQueue();
	midiHandler.init();
}

void Rack::setConfigPath(std::string path) {
	configPath = path;
}

void Rack::initialConfig() {
	rackConfig.system.threads = { 120, 6, 120 };
}

/* TODO:
 * Move this config out into a separate set of objects
 */
std::string Rack::loadConfig() {
	std::ifstream fcfg;
	std::string config = "", line = "";
	fcfg.open(configPath);
	if(fcfg.fail()) {
		std::cerr << "Config file `" << configPath <<"` not found!" << std::endl;
		exit(1);
	}
	while(fcfg) {
		std::getline(fcfg, line);
		config += line;
	}
	fcfg.close();
	return config;
}

void Rack::parseConfig(picojson::value v, RConfigArea area) {
	picojson::value cv;
	if(v.is<picojson::object>()) {
		const picojson::object& o = v.get<picojson::object>();
			for (picojson::object::const_iterator i = o.begin(); i != o.end(); ++i) {
				if(i->first == "system" && area == ROOT)
					parseConfig(i->second, SYSTEM);
				else
				if(i->first == "rack" && area == ROOT)
					parseRack(i->second);
				else
				if(i->first == "midi" && area == ROOT) {
					const picojson::object& mobj = i->second.get<picojson::object>();
					for (picojson::object::const_iterator mit = mobj.begin(); mit != mobj.end(); ++mit)
						midiHandler.addModule(mit->second.get<std::string>(), mit->first);
				}
				else
				if(i->first == "threads" && area == SYSTEM) {
					cv = i->second.get("workers");
					if(!cv.is<picojson::null>())
						rackConfig.system.threads.workers = (int)(cv.get<double>());

					cv = i->second.get("worker_us");
					if(!cv.is<picojson::null>())
						rackConfig.system.threads.worker_us = (int)(cv.get<double>());

					cv = i->second.get("cycle");
					if(!cv.is<picojson::null>())
						rackConfig.system.threads.cycle = (int)(cv.get<double>());
					return;
				}
		}
	}
}

void Rack::parseRack(picojson::value v) {
	using namespace picojson;
	Plug *plug;
	Jack *jack;
	std::string fm, pl, to, jk;

	std::vector<ConfigConnection> connections;

	value cv;

	// get the virtual mainlines plugs
	auto mlines = v.get("mainlines").get<PICO::array>();
	for(PICO::array::const_iterator mit = mlines.begin(); mit != mlines.end(); ++mit) {
		plug = new Plug(NULL);
		plug->name = (*mit).get("plug").get<std::string>();
		plugArray.push_back(plug);
	}

	// get the daisy chains
	auto daisychains = v.get("daisychains").get<PICO::array>();
	for(PICO::array::const_iterator it = daisychains.begin(); it != daisychains.end(); ++it) {
		fm = (*it).get("from").get<std::string>();
		pl = (*it).get("plug").get<std::string>();
		to = (*it).get("to").get<std::string>();
		jk = (*it).get("jack").get<std::string>();

		auto unitTo = parseUnit(to, v.get(to));

		if(fm == "rack") {
			plug = getPlug(pl);
			jack = unitTo->getJack(jk);
			plug->jack = jack;
			plug->connected = jack->connected = true;
			continue;
		}

		auto unitFrom = parseUnit(fm, v.get(fm));
		unitFrom->setConnection(pl, jk, unitTo.get());

	}
}

std::shared_ptr<RackUnit> Rack::parseUnit(std::string name, PICO::value config) {
	using namespace picojson;

	std::shared_ptr<RackUnit> rack;
	std::unique_ptr<RackUnit> uq;
	std::string target;
	bool dynamic = false;

	if((rack = rackChain.getUnit(name)) != nullptr)
		return rack;


	auto cv = config.get("library");

	if(!cv.is<PICO::null>()) {
		dynamic = true;
		target = cv.get<std::string>();
	}

	if(!dynamic)
		uq = unitFactory->build(config.get("unit").get<std::string>(), name);
	else {
		uq = unitFactory->load(target, config.get("unit").get<std::string>(), name);

	}
	if(uq == nullptr)
		return nullptr;

	rack = std::shared_ptr<RackUnit>(uq.release());
	cv = config.get("bindings");
	if(rack->midiControllable() && !cv.is<PICO::null>())
		parseBindings(rack, cv);

	cv = config.get("config");
	if(!cv.is<PICO::null>()) {
		auto cfgOptions = cv.get<object>();
		for (object::const_iterator it = cfgOptions.begin(); it != cfgOptions.end(); ++it)
			rack->setConfig(it->first, it->second.get<std::string>());
	}
	rackChain.addUnit(rack);
	return rack;
}

void Rack::parseBindings(std::shared_ptr<RackUnit> unit, picojson::value cv) {

	std::map<std::string, std::function< void(int) > > exported = unit->midiExportedMethods();
	std::map<std::string, std::function< void(int) > >::iterator mit;

	const picojson::object& bindings = cv.get<picojson::object>();
	for (picojson::object::const_iterator bit = bindings.begin(); bit != bindings.end(); ++bit) {
		for(mit = exported.begin(); mit != exported.end(); mit++)

			if(mit->first == bit->first) {
				if(bit->second.get("module").is<picojson::null>() || bit->second.get("code").is<picojson::null> ())
					continue;

				std::string module = bit->second.get("module").get<std::string>();
				double code = bit->second.get("code").get<double>();
				midiHandler.addBinding(module, code, mit->second);
			}
	}
}

void Rack::initRackQueue() {
	mRackQueue = new RackQueue(rackConfig.system.threads.workers);
	mRackQueue->init();
}

void Rack::start() {
	rackState = RACK_AC;
	rackChain.setRackQueue(mRackQueue);
	mCycleThread = new std::thread(&Rack::cycle, this);
	eventLoop.addEventListener(FwProcComplete, std::bind(&Rack::onCycleEvent, this, std::placeholders::_1));
	eventLoop.start();
	midiHandler.start();

	// warm up cycle
	std::cout << "Warm up cycle..." << std::endl;
	mCycleCount++;
	mCycleCondition.notify_one();
}

void Rack::cycle() {

	std::mutex m;
	std::unique_lock<std::mutex> ul(m);

	while(1) {
		if(mCycleCount == 0)
			mCycleCondition.wait(ul);

		RACK_TELEMETRY(metricUnitCycleStart, std::chrono::steady_clock::now());
		for(auto plug : plugArray) {
			if(!plug->connected)
				continue;
			plug->jack->rackFeed(RackState::RACK_AC);

		}
		RACK_TELEMETRY(metricUnitCycleEnd, std::chrono::steady_clock::now());
		mCycleCount--;
	}

}

Plug *Rack::getPlug(std::string name) const {
	int sz = plugArray.size();
	for(int i = 0; i < sz; i++)
		if(plugArray[i]->name == name)
			return (plugArray[i]);

	return NULL;
}


void Rack::setRackUnitFactory(std::unique_ptr<RackUnitGenericFactory> factory) {
	unitFactory = std::move(factory);
	unitFactory->setEventLoop(&eventLoop);
}

void Rack::initEvents(int numEvents) {
	eventLoop.initEvents(numEvents);
}

std::map<std::string, std::shared_ptr<RackUnit> > Rack::getUnits() {
	return rackChain.getUnits();
}

std::shared_ptr<RackUnit> Rack::getUnit(std::string name) {
	return rackChain.getUnit(name);
}

EventLoop *Rack::getEventLoop() {
	return &eventLoop;
}

void Rack::onCycleEvent(std::shared_ptr<EventMessage> msg) {
	//cycle();
	mCycleCount++;
	mCycleCondition.notify_one();
}



// Telemetry
#if RACK_METRICS
void Rack::cbmetricUnitCycle(
		std::function<void(std::chrono::steady_clock::time_point)> start,
		std::function<void(std::chrono::steady_clock::time_point)> end
) {
	metricUnitCycleStart = start;
	metricUnitCycleEnd = end;
}
#endif
