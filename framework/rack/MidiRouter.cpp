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
#include "MidiRouter.h"

using namespace RackoonIO;

void MidiRouter::addModule(string port, string name) {
	modules.push_back(new MidiModule(port, name));
}

void MidiRouter::init() {
	for(std::vector<MidiModule*>::iterator it = modules.begin(); it != modules.end(); ++it) {
		if(!(*it)->init())
			it = modules.erase(it);

		if(it == modules.end())
			break;
	}
}

void MidiRouter::cycle() {
	for(std::vector<MidiModule*>::iterator it = modules.begin(); it != modules.end(); ++it)
		(*it)->cycle();
}
MidiModule* MidiRouter::operator[] (std::string name) {

	for(std::vector<MidiModule*>::iterator it = modules.begin(); it != modules.end(); ++it)
		if((*it)->getAlias() == name)
			return *it;

	return nullptr;
}

void MidiRouter::addBinding(std::string module, double code, std::function<void(int)> func) {
	for(std::vector<MidiModule*>::iterator it = modules.begin(); it != modules.end(); ++it)
		if((*it)->getAlias() == module)
			(*it)->addBinding(code, func);
}
