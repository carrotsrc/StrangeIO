#include "framework/component/unit_loader.hpp"

using namespace strangeio::component;

unit_uptr unit_loader::load(std::string model, std::string label, std::string target) {
	auto lib = strangeio::library::load(target);
	if(!lib) {
		return nullptr;
	}
	auto loader = lib->load_symbol<UnitBuilderPtr>(std::string("Build")+model);
	
	if(loader == nullptr) {
		return nullptr;
	}
	
	
	auto u = loader(label);
	return unit_uptr(u);
}
