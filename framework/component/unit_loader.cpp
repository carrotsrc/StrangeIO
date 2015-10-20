#include "framework/component/unit_loader.hpp"

using namespace strangeio::component;

unit_uptr unit_loader::load(std::string model, std::string label, std::string target) {

	auto it = m_libraries.find(target);
	if(it == m_libraries.end()) {
		auto libptr = strangeio::library::load(target);
		if(!libptr) return nullptr;
		auto p = m_libraries.insert(std::make_pair(target, std::move(libptr)));
		it = p.first;
	}

	auto& lib = it->second;
	
	auto loader = lib->load_symbol<UnitBuilderPtr>(std::string("Build")+model);
	
	if(loader == nullptr) {
		return nullptr;
	}
	
	
	auto u = loader(label);
	
	return unit_uptr(u);
}
