/*
 * Copyright (C) 2015  Charlie Fyvie-Gauld
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
#ifndef LV2PORT_H
#define LV2PORT_H

namespace strangeio {
namespace Hosting {

struct LV2Port {
	enum PortType {
		Input,
		Output,
		midi,
	};

	const std::string symbol;
	const PortType type;
	const uint32_t index;
	const LilvPort *port;
};


} // Hosting
} // StrangeIO

#endif
