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
namespace RackoonIO {
typedef struct {
	int cycle;

	int workers;
	int worker_us;
} ThreadsConfig;

typedef struct {
	ThreadsConfig threads;
} SystemConfig;

typedef struct {
	SystemConfig system;
} RackConfig;

enum RConfigArea {
	ROOT,
	SYSTEM,
	RACK,
};

typedef struct {
	string plug;
	string jack;
	string unit;
	string name;
} ConfigConnection;

}
#endif
