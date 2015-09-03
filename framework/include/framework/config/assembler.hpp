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
#ifndef __RACKASSEMBLER_H_1439213854__
#define __RACKASSEMBLER_H_1439213854__
#include "framework/common.h"
#include "framework/rack/Rack.h"
#include "framework/rack/RackUnit.h"
#include "framework/factories/RackUnitGenericFactory.h"

#include "RackDesc.h"

namespace strangeio {
namespace Config {

using RackUnitFactory = strangeio::RackUnitGenericFactory;
using UnitDesc = RackDesc::Setup::Unit;


class RackAssembler {
public:
	RackAssembler(std::unique_ptr<RackUnitFactory>);
	void assemble(const RackDesc& desc, Rack& rack);

	std::unique_ptr<RackUnit> assembleUnit(std::string unit, std::string label, std::string target);

private:
	std::unique_ptr<RackUnitFactory> mUnitFactory;

	void assembleMainlines(const RackDesc& desc, Rack& rack);
	void assembleDaisychains(const RackDesc& desc, Rack& rack);
	void assembleMidiBindings(const RackDesc& desc, Rack& rack, RackUnit& unit);
	void assembleMidiDevices(const RackDesc& desc, Rack& rack);

	const UnitDesc& unitDescription(const RackDesc& desc, std::string label);
	void checkUnit(const RackDesc& desc, Rack& rack, std::string label);
	void sizeRackQueue(const RackDesc& desc, Rack& rack);
};

} // Config
} // StrangeIO;
#endif

