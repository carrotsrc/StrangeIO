#include <fstream>
#include "Rack.h"

using namespace RackoonIO;
Rack::Rack() {
	rackState = RACK_OFF;
	configPath = "rackoonio.cfg";
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
		cout << err << endl;
	initRackQueue();
	midiRouter.init();
	uSleep = std::chrono::microseconds(rackConfig.system.threads.cycle);
}

void Rack::setConfigPath(std::string path) {
	configPath = path;
}

void Rack::initialConfig() {
	rackConfig.system.threads = { 120, 6, 120 };
}

std::string Rack::loadConfig() {
	std::ifstream fcfg;
	std::string config = "", line = "";
	fcfg.open(configPath);
	if(fcfg.fail()) {
		cerr << "Config file `" << configPath <<"` not found!" << endl;
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
						midiRouter.addModule(mit->second.get<std::string>(), mit->first);
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
	RackUnit *unitFrom, *unitTo;

	const object& o = v.get<picojson::object>();
	vector<ConfigConnection> connections;

	value cv;

	// get the virtual mainlines plugs
	PICO::array mlines = v.get("mainlines").get<PICO::array>();
	for(PICO::array::const_iterator mit = mlines.begin(); mit != mlines.end(); ++mit) {
		plug = new Plug(NULL);
		plug->name = (*mit).get("plug").get<std::string>();
		plugArray.push_back(plug);
	}

	// get the daisy chains
	PICO::array daisychains = v.get("daisychains").get<PICO::array>();
	for(PICO::array::const_iterator it = daisychains.begin(); it != daisychains.end(); ++it) {
		unitFrom = unitTo = nullptr;
		fm = (*it).get("from").get<std::string>();
		pl = (*it).get("plug").get<std::string>();
		to = (*it).get("to").get<std::string>();
		jk = (*it).get("jack").get<std::string>();

		unitTo = parseUnit(to, v.get(to));
		
		if(fm == "rack") {
			plug = getPlug(pl);
			jack = unitTo->getJack(jk);
			plug->jack = jack;
			plug->connected = jack->connected = true;
			continue;
		}
		
		unitFrom = parseUnit(fm, v.get(fm));
		unitFrom->setConnection(pl, jk, unitTo);

	}
}

RackUnit *Rack::parseUnit(std::string name, PICO::value config) {
	using namespace picojson;

	RackUnit *rack = nullptr;
	std::unique_ptr<RackUnit> uq;
	value cv;
	
	if((rack = rackChain.getUnit(name)) != nullptr)
		return rack;

	uq = unitFactory->build(config.get("unit").get<std::string>(), name);
	if(uq == nullptr)
		return nullptr;

	rack = uq.release();
	cv = config.get("bindings");
	if(rack->midiControllable() && !cv.is<PICO::null>())
		parseBindings(rack, cv);

	cv = config.get("config");
	if(!cv.is<PICO::null>()) {
		const object& cfgOptions = cv.get<object>();
		for (object::const_iterator it = cfgOptions.begin(); it != cfgOptions.end(); ++it)
			rack->setConfig(it->first, it->second.get<std::string>());
	}
	rackChain.addUnit(rack);
	return rack;
}

void Rack::parseBindings(RackUnit *unit, picojson::value cv) {

	std::map<string, std::function< void(int) > > exported = unit->midiExportedMethods();
	std::map<string, std::function< void(int) > >::iterator mit;

	const picojson::object& bindings = cv.get<picojson::object>();
	for (picojson::object::const_iterator bit = bindings.begin(); bit != bindings.end(); ++bit) {
		for(mit = exported.begin(); mit != exported.end(); mit++)

			if(mit->first == bit->first) {
				const picojson::object& bind = bit->second.get<picojson::object>();

				if(bit->second.get("module").is<picojson::null>() || bit->second.get("code").is<picojson::null> ()) 
					continue;

				std::string module = bit->second.get("module").get<std::string>();
				double code = bit->second.get("code").get<double>();
				midiRouter.addBinding(module, code, mit->second);
			}
	}
}

void Rack::initRackQueue() {
	rackQueue = new RackQueue(rackConfig.system.threads.workers);
	rackQueue->init();
	rackQueue->setSleep(std::chrono::microseconds(rackConfig.system.threads.worker_us));
}

void Rack::start() {
	rackState = RACK_AC;
	this->rackChain.setRackQueue(rackQueue);
	cycleThread = new std::thread(&Rack::cycle, this);
}

void Rack::cycle() {
	std::vector<Plug*>::iterator it;
	Plug *plug = NULL;
	while(rackState == RACK_AC) {
		int sz = plugArray.size();
		for(int i = 0; i < sz; i++) {
			if(!plugArray[i]->connected)
				continue;
			plugArray[i]->jack->rackFeed(RackState::RACK_AC);

		}
		rackQueue->cycle();
		midiRouter.cycle();
		std::this_thread::sleep_for(uSleep);
	}
}

Plug *Rack::getPlug(string name) const {
	int sz = plugArray.size();
	for(int i = 0; i < sz; i++)
		if(plugArray[i]->name == name)
			return (plugArray[i]);

	return NULL;
}


void Rack::setRackUnitFactory(unique_ptr<RackUnitGenericFactory> factory) {
	unitFactory = std::move(factory);
}

void Rack::initEvents(int numEvents) {
	eventLoop.initEvents(numEvents);
}
