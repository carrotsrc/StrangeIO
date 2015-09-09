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
#include <iostream>
#include "framework/midi/midi_handler.hpp"

using namespace strangeio::midi;

midi_handler::midi_handler(driver_utility* interface) :
m_interface(interface)
{ }

void midi_handler::add_device(std::string port, std::string name) {
	m_devices.push_back(device(port, name, m_interface));
}

void midi_handler::init() {
	for(auto& dev : m_devices) {
		if(!dev.init()) continue;
		m_active_dev.push_back(&dev);
	}
}

void midi_handler::start() {
	for(auto& module : m_active_dev) {
		module->start();
	}
}

void midi_handler::stop() {
	
	for(auto& ref : m_active_dev) {
		ref->stop();
	}
}
device* midi_handler::operator[] (std::string name) {

	for(auto& dev : m_active_dev) {
		if(dev->get_alias() == name)
			return dev;
	}
	/*
	for(auto it = m_devices_dev.begin(); it != m_devices.end(); ++it)
		if((*it).get_alias() == name)
			return &(*it);
	*/
	return nullptr;
}

void midi_handler::add_binding(std::string module, double code, std::function<void(msg)> func) {

	for(auto& ref : m_devices) {
		if(ref.get_alias() == module)
			ref.add_binding(code, func);
	}
}

std::vector<device>& midi_handler::devices() {
	return m_devices;
}
