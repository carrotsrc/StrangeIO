#include "RackUnitFactory.h"
#include "system/units/RuFlacLoad.h"
#include "system/units/RuAlsa.h"
#include "system/units/RuEcho.h"
#include "system/units/RuLevels.h"
using namespace RackoonIO;

std::unique_ptr<RackUnit> RackUnitFactory::build(std::string type, std::string name) {
	RackUnit *unit = NULL;
	if(type == "FlacLoad")
		unit = (RackUnit*) new RuFlacLoad();
	else
	if(type == "Alsa")
		unit = (RackUnit*) new RuAlsa();
	else
	if(type == "Echo")
		unit = (RackUnit*) new RuEcho();
	else
	if(type == "Levels")
		unit = (RackUnit*) new RuLevels();
	else
		return nullptr;

	unit->setName(name);

	return std::unique_ptr<RackUnit>(unit);
}
