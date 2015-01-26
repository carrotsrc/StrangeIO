#ifndef RACK_H
#define RACK_H
#include "RackChain.h"
#include "framework/threads/RackQueue.h"
#include "RackConfig.h"
#include "framework/picojson/picojson.h"

namespace RackoonIO {
class Rack {
	RackConfig rackConfig;
	RackQueue *rackQueue;
	std::string loadConfig();
	void parseConfig(picojson::value, RConfigArea);
	void initialConfig();

	void initRackQueue();
public:
	Rack();
	void init();
};
}
#endif 
