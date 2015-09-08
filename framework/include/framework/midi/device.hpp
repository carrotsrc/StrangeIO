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
#ifndef __MIDI_DEVICE_HPP__
#define __MIDI_DEVICE_HPP__

#include <thread>
#include <map>

#include "framework/fwcommon.hpp"
#include "framework/midi/driver_utility.hpp"
#include "framework/midi/input_handle.hpp"

namespace strangeio {
namespace midi {

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
class device {
public:

	/** Setup the device
	 *
	 * @param port The unique ID of the port
	 * @param alias The system alias of the device
	 */
	device(std::string port, std::string alias, driver_utility* interface);
	/** Initialiases the device by opening the port
	 *
	 * @return true on success; false on error
	 */
	bool init();
	/** Method run in thread to cycle any queued MIDI messages and call any bindings
	 */
	void cycle();
	
	bool running() const;

	/** get the unique port ID
	 *
	 * @return The string port name assigned to the module
	 */
	std::string get_port() const;

	/** get the alias assigned to the module
	 *
	 * @return The string alias assigned to the module
	 */
	std::string get_alias() const;

	/** Add a binding to a controller
	 *
	 * Use this method to bind a callback to a specific MIDI controller
	 */
	void add_binding(double, std::function<void(msg)>);

	const std::map<int, std::function<void(msg)> >& get_bindings();

	/** Start the module cycle thread */
	void start();

	/** Stop the module cycle thread */
	void stop();

#if DEVBUILD
	void close_handle();
	void test_cycle();
#endif

private:
	driver_utility *m_interface;
	midi_in_uptr m_handle;
	std::string m_port_name; ///< Unique ID of port handle (e.g. hw 1,0,0 )
	std::string m_alias; ///< Device alias in the system
	std::thread m_thread; ///< Thread used to cycle MIDI signals
	bool m_running; ///< Flag to signify the running state of thread

	/** An map of exported callbacks bound to MIDI controller */
	std::map<int, std::function<void(msg)> > m_bindings;
};

} // Midi
} // StrangeIO
#endif
