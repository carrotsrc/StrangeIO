#include "RackAssembler.h"
using namespace StrangeIO;
using namespace StrangeIO::Config;

RackAssembler::RackAssembler(std::unique_ptr<RackUnitFactory> factory) {
	mUnitFactory = std::move(factory);
}

void RackAssembler::assemble(const RackDesc& desc, Rack& rack) {
	
}

void RackAssembler::assembleMainlines(const RackDesc& desc, Rack& rack) {
	for(auto mainline : desc.setup.mainlines) {
		rack.addMainline(mainline);
	}
}

void RackAssembler::assembleDaisychains(const RackDesc& desc, Rack& rack) {
	for(auto link : desc.setup.daisychains) {
		auto& udFrom = unitDescription(desc, link.from);
		auto unitFrom = assembleUnit(udFrom.unit, udFrom.label, udFrom.library); 
	}
}

const UnitDesc& RackAssembler::unitDescription(const RackDesc& desc, std::string label) {
	for(const auto& unit : desc.setup.units) {
		if(unit.label == label) {
			return unit;
		}
	}

	throw;
}

std::unique_ptr<RackUnit> RackAssembler::assembleUnit(std::string unit, std::string label, std::string target) {
	if(target.empty()) {
		return mUnitFactory->build(unit, label);
	} else {
		return mUnitFactory->load(target, unit, label);
	}
}
