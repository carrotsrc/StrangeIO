#include "framework/rack/Rack.h"
#include "framework/memory/BitfieldCache.h"
#include "RackUnitFactory.h"

int main (int argc, char* argv[])
{
	std::cout << "StrangeIO Example Code" << std::endl;
	std::string userConfig = "";
	if(argc > 0) {
		for(int i = 0; i < argc; i++) {
			if(strcmp(argv[i], "-c") == 0 && argc > 1)
				userConfig = std::string(argv[i+1]);
		}
	}
	StrangeIO::Rack rack;
	StrangeIO::GenericEventMessageFactory msgFactory;
	if(userConfig != "") {
		std::cout << "Using config: " << userConfig << std::endl;
		rack.setConfigPath(userConfig);
	}

	std::unique_ptr<StrangeIO::RackUnitGenericFactory> factory(new ExampleCode::RackUnitFactory());

	StrangeIO::CacheHandler *cache = new StrangeIO::BitfieldCache();
	cache->init(512, 50);
	factory->setCacheHandler(cache);
	factory->setMessageFactory(&msgFactory);

	rack.setRackUnitFactory(std::move(factory));
	rack.init();
	rack.initEvents(0);
	rack.start();

	    std::map<std::string, StrangeIO::RackUnit*> units;
	units = rack.getUnits();
	std::map<std::string, StrangeIO::RackUnit*>::iterator it;

	int x;
	std::cin >> x;
}
