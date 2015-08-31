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
#include "framework/midi/MidiHandler.hpp"

using namespace StrangeIO::Midi;

MidiHandler::MidiHandler(DriverUtilityInterface* interface) :
m_interface(interface)
{ }

void MidiHandler::add_module(std::string port, std::string name) {
	m_modules.push_back(MidiDevice(port, name, m_interface));
}

void MidiHandler::init() {
	for(auto it = m_modules.begin(); it != m_modules.end(); ++it) {
		if(!(*it).init())
			it = m_modules.erase(it);
	}
}

void MidiHandler::start() {
	for(auto& module : m_modules) {
		module.start();
	}
}

void MidiHandler::stop() {
	
	for(auto& ref : m_modules) {
		ref.stop();
	}
}
MidiDevice* MidiHandler::operator[] (std::string name) {

	for(auto it = m_modules.begin(); it != m_modules.end(); ++it)
		if((*it).get_alias() == name)
			return &(*it);

	return nullptr;
}

void MidiHandler::add_binding(std::string module, double code, std::function<void(MidiCode)> func) {

	for(auto& ref : m_modules) {
		if(ref.get_alias() == module)
			ref.add_binding(code, func);
	}
}

const std::vector<MidiDevice>& MidiHandler::get_modules() {
	return m_modules;
}
