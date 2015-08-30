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
#include "framework/midi/MidiDevice.hpp"
#include <iomanip>

using namespace StrangeIO::Midi;

MidiDevice::MidiDevice(std::string port, std::string alias, DriverUtilityInterface* interface) :
m_interface(interface), m_handle(nullptr), m_port_name(port), m_alias(alias)
{ }

bool MidiDevice::init() {
	if((m_handle = m_interface->open_input_port(m_alias, m_port_name)) == false)
		return false;

	return true;
}

void MidiDevice::cycle() {
	while(m_running) {
		MidiCode code = m_handle->flush();

		if(code.f == 0)
			return;

		try { m_bindings.at(code.n)(code); }
		catch(const std::out_of_range& oor) {}
	}
}

std::string MidiDevice::get_alias() {
	return m_alias;
}

std::string MidiDevice::get_port() {
	return m_port_name;
}

void MidiDevice::add_binding(double code, std::function<void(MidiCode)> func) {
	m_bindings.insert(std::pair< int, std::function<void(MidiCode)> >((int)code, func));
}
const std::map<int, std::function<void(MidiCode)> >& MidiDevice::get_bindings() {
	return m_bindings;
}

void MidiDevice::start() {
	m_running = true;
	m_thread = std::thread(&MidiDevice::cycle, this);
}

void MidiDevice::stop() {
	m_running = false;
	m_thread.join();
	
}
