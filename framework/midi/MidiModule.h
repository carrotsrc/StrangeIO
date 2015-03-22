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
#ifndef MIDIMODULE_H
#define MIDIMODULE_H
#include "common.h"
namespace RackoonIO {


/** Basic representation of a MIDI control code */
typedef struct {
	char f; ///< function code
	char n; ///< note code
	char v; ///< Velocity of note
} MidiCode;


/** A representation and handle of a MIDI device
 *
 * When a device is set in the configuration, a MidiModule object
 * is created as the handle and interface for the device.
 *
 * When an object is configured as bound to a code, the callback is
 * passed to the respective MIDI module.
 *
 * The module then deals with sending the velocity of the MIDI signal 
 * to the RackUnit that is bound to that code.
 *
 * The module gets any messages from the port handle on each cycle
 */
class MidiModule {
	snd_rawmidi_t *inMidi; ///< input port handle
	std::string portName; ///< Unique ID of port handle (e.g. hw 1,0,0 )
	std::string alias; ///< Device alias in the system
	std::thread mThread; ///< Thread used to cycle MIDI signals
	bool mRunning; ///< Flag to signify the running state of thread
	char buffer[3]; ///< buffer for a MIDI control message

	/** An map of exported callbacks bound to MIDI controller */
	std::map<int, std::function<void(int)> > bindings;
public:
	/** Setup the device
	 *
	 * @param port The unique ID of the port
	 * @param alias The system alias of the device
	 */
	MidiModule(string port, string alias);

	/** Initialiases the device by opening the port
	 *
	 * @return true on success; false on error
	 */
	bool init();

	/** Method run in thread to cycle any queued MIDI messages and call any bindings
	 */
	void cycle();

	/** Retrieve a single MIDI message if it is queued in the driver buffer
	 *
	 * @return The a filled out MidiCode if one is waiting; otherwise a zeroed MidiCode structure
	 */
	MidiCode flush();

	/** get the unique port ID
	 *
	 * @return The string port name assigned to the module
	 */
	std::string getPort();

	/** get the alias assigned to the module
	 *
	 * @return The string alias assigned to the module
	 */
	std::string getAlias();

	/** Add a binding to a controller
	 *
	 * Use this method to bind a callback to a specific MIDI controller
	 */
	void addBinding(double, std::function<void(int)>);

	/** Start the module cycle thread */
	void start();

	/** Stop the module cycle thread */
	void stop();
};

}
#endif
