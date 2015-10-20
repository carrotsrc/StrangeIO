#include <iostream>

#include "framework/fwcommon.hpp"
#include "framework/component/mount.hpp"

using namespace strangeio::component;

mount::mount() {
}

mount::mount(const mount& orig) {
}

mount::~mount() {
}


void mount::add_unit(unit_uptr unit) {

    mount_dependencies(unit.get());
    auto label = unit->ulabel();
    m_mounted.insert(
    std::pair<std::string, unit_sptr>(
	label, unit_sptr(std::move(unit))
	)
    );
}

const std::map<std::string, unit_sptr> & mount::get_units() {
	return m_mounted;
}

unit_wptr mount::get_unit(std::string label) {

	auto u = m_mounted.find(label);

	if(u == m_mounted.end()) {
		return unit_wptr();
	}

	return u->second;
}

bool mount::has_unit(std::string label) {
	auto u = m_mounted.find(label);
	if(u == m_mounted.end()) return false;

	return true;
}

void mount::clear_units() {
	m_mounted.clear();
}

void mount::add_mainline(std::string name) {
	if(m_mainlines.find(name) != m_mainlines.end())
		return;

	m_mainlines.insert(
			std::pair<std::string, unit*>(
				name, nullptr
			)
		);
}

bool mount::connect_mainline(std::string mainline, std::string unit) {

	auto line = m_mainlines.find(mainline);

	if(line == m_mainlines.end()) return false;

	auto wptr = get_unit(unit);
	if(wptr.expired()) return false;

	line->second = wptr.lock().get();
	m_raw_mainlines.push_back(line->second);

#if DEVBUILD
	std::cout << "[Connection]\t" << 
		"rack." << mainline << 
		" --> " <<
		unit << ".power" << std::endl;
#endif
	return true;
}

bool mount::connect_units(std::string from, std::string out, std::string to, std::string in) {
	auto ufrom = get_unit(from);
	if(ufrom.expired()) return false;


	auto uto = get_unit(to);
	if(uto.expired()) return false;

	auto from_shr = ufrom.lock();
	auto to_shr = uto.lock();

	auto output_id = from_shr->has_output(out);
	auto input_id = to_shr->has_input(in);

	if(output_id < 0 || input_id < 0) return false;


	auto link_in = const_cast<LinkIn*>(to_shr->get_input(input_id));
	auto ret = from_shr->connect(output_id, link_in);
	
#if DEVBUILD
	if(ret) {
		std::cout << "[Connection]\t" << 
			from << "." << out << 
			" --> " <<
			to << "." << in << std::endl;
	} else {
		std::cout << "[Connection failure]" << std::endl;
	}
#endif
	
	return ret;
}
