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

/** An object for managing the devices
 *
 * This class is keeps the array of MIDI device
 * modules that are used in the system and manages to
 * the binding routes from the configuration. It also
 * controls the cycles of the modules.
 *
 * @note
 * The callback has a signature of:<br /><br />
 * std::function<void(int)>
 */
class MidiRouter {
	bool active; ///< A flag showing activity
	std::vector<MidiModule*> modules; ///< A vector pointer to MidiModule objects

public:
	/** Add a new module to the router
	 *
	 * This is where a new module is initialised, given the port name and alias
	 *
	 * @param portName The port name to be used for retrieving the handle
	 * @param portAlias The alias used by the device
	 */
	void addModule(std::string, std::string);

	/** Create a new binding to a MIDI controller
	 *
	 * Pass in the callback to be bound to a device and controller code.
	 *
	 * @param module The module alias to bind to
	 * @param code The controller code to bind to
	 * @param func The callback function to be bound
	 */
	void addBinding(std::string, double, std::function<void(int)>);
	MidiModule* operator[] (std::string);

	/** Framework method used to initiliase the router and manager */
	void init();

	/** Framework method called to cycle the devices */
	void cycle();
};

}
#endif
