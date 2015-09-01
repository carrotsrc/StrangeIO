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
#ifndef RACKCONFIG_H
#define RACKCONFIG_H
namespace strangeio {
/** @todo These need to be moved into their own namespace */

/** A configuration structure for threads, reflecting the JSON */
typedef struct {
	int cycle; ///< The time to sleep between main rack cycles

	int workers; ///< Number of worker threads in the pool
	int worker_us; ///< Time for threads to sleep between checks for packages
} ThreadsConfig;

/** A configuration structure for the system, reflecting the JSON */
typedef struct {
	ThreadsConfig threads; ///< The config for the threads
} SystemConfig;

/** The root configuration structure **/
typedef struct {
	SystemConfig system; ///< The system configuration structure
} RackConfig;

//** Areas of the configuration file */
enum RConfigArea {
	ROOT, ///< The root document
	SYSTEM, ///< The System configuration
	RACK, ///< The rack configuration
};

/** A structure representing the configuration of a unit connector **/
typedef struct {
	std::string plug; ///< From unit (jack)plug
	std::string jack; ///< To unit jack(socket)
	std::string unit; ///< To unit type
	std::string name; ///< To unit name
} ConfigConnection;

}
#endif
