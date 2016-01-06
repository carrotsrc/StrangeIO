
#include <vector>
#include "framework/fwcommon.hpp"
#include "framework/component/registry.hpp"

using namespace strangeio::component;

registry::registry()
	: registry_utility()
	, m_current_id(0)
{}

registry::~registry() {
}

void registry::register_component(registrable& cmpt) {
	std::lock_guard<std::mutex> lock(m_mutex);
	
	cmpt.set_id(m_current_id);
	rhandle i;
	i.label  = cmpt.ulabel();
	i.model = cmpt.umodel();
	i.type = cmpt.type();
	
	cmpt.set_handle(i);
	
	m_registry.push_back(i);
	strangeio::log::inst()
		<< "Registered [" << m_current_id << "] "
		<< i.model << "::" << i.label << strangeio::lendl;
	m_current_id++;
						
}

std::vector<rhandle>::const_iterator registry::begin() {
	return m_registry.cbegin();
}

std::vector<rhandle>::const_iterator registry::end() {
	return m_registry.cend();
}

rhandle registry::with_id(long id) {
	return m_registry.at(id);
}

