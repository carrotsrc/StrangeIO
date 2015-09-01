/* Copyright (C) 2015  Charlie Fyvie-Gauld
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef __RACKDESC_H_1439063050__
#define __RACKDESC_H_1439063050__

#include "framework/common.h"
namespace strangeio {
namespace Config {

typedef struct {

	typedef struct {

		typedef struct {
			int num_workers;
		} Threading;
		Threading threads;

	} System;

	typedef struct {

		typedef struct {
			std::string label;
			std::string port;
		} Controller;

		std::vector<Controller> controllers;

	} midi;

	typedef struct {

		typedef struct {
			std::string from, plug, to, jack;
		} Link;

		typedef struct {

			typedef struct {
				std::string type, value;
			} Config;

			typedef struct {
				std::string name, module;
				int code;
			} Binding;

			std::string label, unit, library;
			std::vector<Config> configs;
			std::vector<Binding> bindings;
		} Unit;

		std::vector<std::string> mainlines;
		std::vector<Link> daisychains;
		std::vector<Unit> units;

	} Setup;

	System system;
	midi midi;
	Setup setup;


} RackDesc;



} // Config
} // StrangeIO
#endif
