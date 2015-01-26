#include <fstream>
#include "Rack.h"

using namespace RackoonIO;
Rack::Rack() {
}

void Rack::init() {
	initialConfig();
	std::string json = loadConfig();
	picojson::value v;
	std::string err = picojson::parse(v, json);
	parseConfig(v, ROOT);
	initRackQueue();
}

void Rack::initialConfig() {
	rackConfig.system.threads = { 120, 6, 120 };
}

std::string Rack::loadConfig() {
	std::ifstream fcfg;
	std::string config = "", line = "";
	fcfg.open("system/rackoonio.cfg");
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

				else if(i->first == "threads" && area == SYSTEM) {
					cv = i->second.get("worker");
					if(!cv.is<picojson::null>())
						rackConfig.system.threads.workers = (int)(cv.get<double>());

					cv = i->second.get("worker_us");
					if(!cv.is<picojson::null>())
						rackConfig.system.threads.worker_us = (int)(cv.get<double>());

					cv = i->second.get("cycle");
					if(!cv.is<picojson::null>())
						rackConfig.system.threads.cycle = (int)(cv.get<double>());

				}
		}
	}
}


void Rack::initRackQueue() {
	rackQueue = new RackQueue(rackConfig.system.threads.workers);
	rackQueue->setSleep(
		std::chrono::microseconds(rackConfig.system.threads.worker_us)
		);
}
