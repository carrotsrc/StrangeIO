#include "framework/rack/Rack.h"
#include "framework/memory/BitfieldCache.h"
#include "RackUnitFactory.h"

int main (int argc, char* argv[])
{
	std::cout << "RackoonIO Example Code" << std::endl;
	std::string userConfig = "";
	if(argc > 0) {
		for(int i = 0; i < argc; i++) {
			if(strcmp(argv[i], "-c") == 0 && argc > 1)
				userConfig = std::string(argv[i+1]);
		}
	}
	RackoonIO::Rack rack;
	if(userConfig != "") {
		cout << "Using config: " << userConfig << endl;
		rack.setConfigPath(userConfig);
	}

	std::unique_ptr<RackoonIO::RackUnitGenericFactory> factory(new RackUnitFactory());
	
	RackoonIO::CacheHandler *cache = new RackoonIO::BitfieldCache();
	cache->init(512, 50);
	factory->setCacheHandler(cache);

	rack.setRackUnitFactory(std::move(factory));
	rack.init();
	rack.initEvents(0);
	rack.start();
	    std::map<std::string, RackoonIO::RackUnit*> units;
	units = rack.getUnits();
	std::map<std::string, RackoonIO::RackUnit*>::iterator it;
	
	int x;
	std::cin >> x;
}
