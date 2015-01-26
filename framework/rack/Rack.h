#ifndef RACK_H
#define RACK_H
#include "RackChain.h"
#include "framework/threads/RackQueue.h"
#include "RackConfig.h"
#include "framework/picojson/picojson.h"

namespace RackoonIO {
class Rack {

	RackConfig rackConfig;
	RackState rackState;
	RackQueue *rackQueue;

	vector<Plug*> plugArray; 

	std::chrono::microseconds uSleep;
	std::thread *cycleThread;

	// config and init
	std::string loadConfig();
	void parseConfig(picojson::value, RConfigArea);
	void parseRack(picojson::value, RConfigArea);
	void initialConfig();
	void initRackQueue();

protected:
	void cycle();

public:
	Rack();
	void init();
	void start();
};
}
#endif 
