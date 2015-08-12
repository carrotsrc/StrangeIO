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
 
#include "framework/common.h"
#include "framework/picojson/picojson.h"
#include "RackDesc.h"

namespace StrangeIO {
namespace Config {

class RackDocument {
public:
	enum Element {
		Root,
		System,
	};

	std::unique_ptr<RackDesc> load(std::string path);

private:
	std::unique_ptr<RackDesc> mRack;
	std::string loadFile(std::string);
	void parseDocument(const picojson::value& v, RackDocument::Element element);
	void parseRack(const picojson::value& v);
	void parseUnit(std::string label, const picojson::value& v);
	void parseBindings(RackDesc::Setup::Unit& unit, const picojson::value & v);
};

} // Config
} // StrangeIO
#endif
