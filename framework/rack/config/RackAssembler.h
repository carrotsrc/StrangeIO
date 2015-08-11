#ifndef __RACKASSEMBLER_H_1439213854__
#define __RACKASSEMBLER_H_1439213854__
#include "framework/common.h"
#include "framework/rack/Rack.h"
#include "framework/rack/RackUnit.h"
#include "framework/factories/RackUnitGenericFactory.h"

#include "RackDesc.h"

namespace StrangeIO {
namespace Config {

using RackUnitFactory = StrangeIO::RackUnitGenericFactory;
using UnitDesc = RackDesc::Setup::Unit;


class RackAssembler {
public:
	RackAssembler(std::unique_ptr<RackUnitFactory>);
	void assemble(const RackDesc& desc, Rack& rack);

	std::unique_ptr<RackUnit> assembleUnit(std::string unit, std::string label, std::string target);

private:
	std::unique_ptr<RackUnitFactory> mUnitFactory;

	void assembleMainlines(const RackDesc& desc, Rack& rack);
	void assembleDaisychains(const RackDesc& desc, Rack& rack);
	void assembleMidiBindings(const RackDesc& desc, Rack& rack, RackUnit& unit);
	void assembleMidiDevices(const RackDesc& desc, Rack& rack);

	const UnitDesc& unitDescription(const RackDesc& desc, std::string label);
	void checkUnit(const RackDesc& desc, Rack& rack, std::string label);
	void sizeRackQueue(const RackDesc& desc, Rack& rack);
};

} // Config
} // StrangeIO;
#endif

