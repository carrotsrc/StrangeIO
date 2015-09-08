/* Copyright 2015 Charlie Fyvie-Gauld
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published
 *  by the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "framework/midi/device.hpp"
#include <iomanip>

using namespace strangeio::midi;

device::device(std::string port, std::string alias, driver_utility* interface) :
m_interface(interface), m_handle(nullptr), m_port_name(port), m_alias(alias)
{ }


bool device::init() {
	if((m_handle = m_interface->open_input_port(m_alias, m_port_name)) == false)
		return false;

	return true;
}
#include <iostream>
void device::cycle() {
#if DEVBUILD
	std::cout << "Midi" << "[" << get_alias() << "]: Started" << std::endl;
#endif
	m_running = true;
	while(m_running) {
		msg code = m_handle->flush();
		if(!m_running) break;

		if(code.f == 0) return;

		try { m_bindings.at(code.n)(code); }
		catch(const std::out_of_range& oor) {}
	}
}

bool device::running() const {
	return m_running;
}

std::string device::get_alias() const {
	return m_alias;
}

std::string device::get_port() const {
	return m_port_name;
}

void device::add_binding(double code, std::function<void(msg)> func) {
	m_bindings.insert(std::pair< int, std::function<void(msg)> >((int)code, func));
}

const std::map<int, std::function<void(msg)> >& device::get_bindings() {
	return m_bindings;
}

void device::start() {
	m_thread = std::thread(&device::cycle, this);
}

void device::stop() {
	m_running = false;
	m_thread.join();
	
}

#if DEVBUILD
void device::close_handle() {
	m_interface->close_input_port(std::move(m_handle));
}

void device::test_cycle() {
	msg code = m_handle->flush();

	try { m_bindings.at(code.n)(code); }
	catch(const std::out_of_range& oor) {}

}
#endif
