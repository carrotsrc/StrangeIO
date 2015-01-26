#include "RackUnitFactory.h"
#include "system/units/RuFlacLoad.h"
#include "system/units/RuAlsa.h"
using namespace RackoonIO;

std::unique_ptr<RackUnit> RackUnitFactory::build(std::string type, std::string name) {
	RackUnit *unit = NULL;
	if(type == "FlacLoad")
		unit = (RackUnit*) new RuFlacLoad();
	else
	if(type == "Alsa")
		unit = (RackUnit*) new RuAlsa();
	else
		return nullptr;

	unit->setName(name);

	return std::unique_ptr<RackUnit>(unit);
}
