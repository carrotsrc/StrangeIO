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
#ifndef __RACKDOCUMENT_H_1439199800__
#define __RACKDOCUMENT_H_1439199800__

#include <memory>
#include <vector>
 
#include "framework/fwcommon.hpp"
#include "framework/picojson/picojson.h"
#include "framework/config/description.hpp"


namespace strangeio {
namespace config {

class document {
public:
	enum element_type {
		root,
		system,
	};

	std::unique_ptr<description> load(std::string path);

private:
	std::unique_ptr<description> m_rack;
	std::string load_file(std::string);
	void parse_document(const picojson::value& v, document::element_type element);
	void parse_rack(const picojson::value& v);
	void parse_unit(std::string label, const picojson::value& v);
	void parse_bindings(description::s_setup::s_unit& unit, const picojson::value & v);
	void parse_leds(description::s_setup::s_unit& unit, const picojson::value & v);
    void parse_system(description::s_setup::s_unit& unit, const picojson::value & v);
	void parse_syslinux(const picojson::value & v);
};

} // Config
} // StrangeIO
#endif
