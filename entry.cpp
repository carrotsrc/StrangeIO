#include "system/units/RuFlacLoad.h"
#include "system/units/RuAlsa.h"
#include "framework/threads/RackQueue.h"
#include "framework/rack/Rack.h"
#include "system/factories/RackUnitFactory.h"

#include <chrono>

int main(void)
{
	RackoonIO::Rack rack;
	std::unique_ptr<RackoonIO::RackUnitGenericFactory> factory(new RackoonIO::RackUnitFactory());
	rack.setRackUnitFactory(std::move(factory));
	rack.init();
	rack.start();
	int x;
	cin >> x;
}




