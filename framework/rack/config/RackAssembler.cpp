#include "RackAssembler.h"
using namespace StrangeIO::Config;
RackAssembler::RackAssembler(std::unique_ptr<RackUnitFactory> factory) {
	mUnitFactory = std::move(factory);
}
void RackAssembler::assemble(const RackDesc & desc, Rack & rack) {
	
}
