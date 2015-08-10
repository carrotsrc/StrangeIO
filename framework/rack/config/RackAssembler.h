#ifndef __RACKASSEMBLER_H_1439213854__
#define __RACKASSEMBLER_H_1439213854__
#include "framework/common.h"
#include "framework/rack/Rack.h"
#include "framework/rack/RackUnit.h"

#include "RackDesc.h"
namespace StrangeIO {
namespace Config {

using RackUnitFactory = StrangeIO::RackUnitGenericFactory;

class RackAssembler {
public:
	RackAssembler(std::unique_ptr<RackUnitFactory>);
	void assemble(const RackDesc & desc, Rack & rack);

private:
	std::unique_ptr<RackUnitFactory> mUnitFactory;
};

} // Config
} // StrangeIO;
#endif

