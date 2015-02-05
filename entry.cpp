#include "system/units/RuFlacLoad.h"
#include "system/units/RuAlsa.h"
#include "framework/threads/RackQueue.h"
#include "framework/rack/Rack.h"
#include "system/factories/RackUnitFactory.h"

#include <chrono>

int main(int argc, char *argv[])
{
	std::string userConfig = "";
	if(argc > 0) {
		for(int i = 0; i < argc; i++) {
			if(strcmp(argv[i], "-c") == 0 && argc > 1)
				userConfig = std::string(argv[i+1]);
		}
	}
	RackoonIO::Rack rack;
	if(userConfig != "")
		rack.setConfigPath(userConfig);
	std::unique_ptr<RackoonIO::RackUnitGenericFactory> factory(new RackUnitFactory());
	rack.setRackUnitFactory(std::move(factory));
	rack.init();
	rack.start();
	int x;
	cin >> x;
}




