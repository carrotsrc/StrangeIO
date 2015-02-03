#ifndef RACK_H
#define RACK_H
#include "RackChain.h"
#include "framework/threads/RackQueue.h"
#include "RackConfig.h"
#include "framework/factories/RackUnitGenericFactory.h"
#include "framework/picojson/picojson.h"
#include "MidiRouter.h"

namespace RackoonIO {
class Rack {

	RackConfig rackConfig;
	RackState rackState;
	RackQueue *rackQueue;

	std::vector<Plug*> plugArray;
	RackChain rackChain;

	MidiRouter midiRouter;

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

	RackUnit *getUnit(std::string);
	std::unique_ptr<RackUnitGenericFactory> unitFactory;

protected:
	void cycle();
	Plug *getPlug(string name) const;
public:
	Rack();

	void setConfigPath(std::string);
	void init();
	void start();

	void setRackUnitFactory(unique_ptr<RackUnitGenericFactory>);
};
}
#endif 
