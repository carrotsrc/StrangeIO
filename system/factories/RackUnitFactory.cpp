#include "RackUnitFactory.h"
#include "system/units/RuFlacLoad.h"
#include "system/units/RuAlsa.h"
#include "system/units/RuEcho.h"
#include "system/units/RuLevels.h"
#include "system/units/RuChannelMixer.h"
#include "system/units/RuPitchBender.h"
#include "system/units/RuLinearInt.h"
#include "system/units/RuLoop.h"
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
	if(type == "ChannelMixer")
		unit = (RackUnit*) new RuChannelMixer();
	else
	if(type == "PitchBender")
		unit = (RackUnit*) new RuPitchBender();
	else
	if(type == "LinearInt")
		unit = (RackUnit*) new RuLinearInt();
	else
	if(type == "Loop")
		unit = (RackUnit*) new RuLoop();
	else
		return nullptr;

	unit->setName(name);
	setDependencies(unit);
	return std::unique_ptr<RackUnit>(unit);
}
