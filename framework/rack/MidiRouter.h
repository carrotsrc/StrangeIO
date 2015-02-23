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
#ifndef MIDIROUTER_H
#define MIDIROUTER_H
#include "MidiModule.h"

namespace RackoonIO {

class MidiRouter {
	bool active;
	std::vector<MidiModule*> modules;

public:
	void addModule(std::string, std::string);
	void addBinding(std::string, double, std::function<void(int)>);
	MidiModule* operator[] (std::string);

	void init();
	void cycle();
};

}
#endif
