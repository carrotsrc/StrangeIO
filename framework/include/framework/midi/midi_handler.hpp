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
#ifndef MIDIHANDLER_H
#define MIDIHANDLER_H
#include <vector>
#include "framework/midi/driver_utility.hpp"
#include "framework/midi/device.hpp"

namespace strangeio {
namespace midi {
	
/** An object for managing the devices
 *
 * This class is keeps the array of MIDI device
 * modules that are used in the system and manages to
 * the binding routes from the configuration. It also
 * starts the MidiModule object's threads for cycling
 * MIDI signals
 *
 * @note
 * The callback has a signature of:<br /><br />
 * std::function<void(int)>
 */
class midi_handler {
public:
	midi_handler(driver_utility* interface);
	/** Add a new module to the router
	 *
	 * This is where a new module is initialised, given the port name and alias
	 *
	 * @param portName The port name to be used for retrieving the handle
	 * @param portAlias The alias used by the device
	 */
	void add_device(std::string, std::string);

	/** Create a new binding to a MIDI controller
	 *
	 * Pass in the callback to be bound to a device and controller code.
	 *
	 * @param module The module alias to bind to
	 * @param code The controller code to bind to
	 * @param func The callback function to be bound
	 */
	void add_binding(std::string, double, std::function<void(msg)>);
	device* operator[] (std::string);

	/** Framework method used to initiliase the router and manager */
	void init();

	/** Start the midi module threads */
	void start();
	/** Stop the midi module threads */
	void stop();

	const std::vector<device>& devices();
	
private:
	bool m_active; ///< A flag showing activity
	std::vector<device> m_devices; ///< A vector pointer to MidiModule objects
	driver_utility* m_interface;
};

} // Midi
} // StrangeIO
#endif
