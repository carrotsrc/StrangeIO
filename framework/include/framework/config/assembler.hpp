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
#include "framework/fwcommon.hpp"
#include "framework/component/rack.hpp"
#include "framework/component/unit.hpp"
#include "framework/component/unit_loader.hpp"

#include "framework/config/description.hpp"

namespace strangeio {
namespace config {

using unit_factory = strangeio::component::unit_loader;
using unit_desc = description::s_setup::s_unit;


class assembler {
public:
	assembler(std::unique_ptr<unit_factory>);
	void assemble(const description& desc, Rack& rack);

	std::unique_ptr<RackUnit> assemble_unit(std::string unit, std::string label, std::string target);

private:
	std::unique_ptr<unit_factory> m_factory;

	void assemble_mainlines(const description& desc, Rack& rack);
	void assemble_daisychains(const description& desc, Rack& rack);
	void assemble_bindings(const description& desc, Rack& rack, RackUnit& unit);
	void assemble_devices(const description& desc, Rack& rack);

	const unit_desc& unit_description(const description& desc, std::string label);
	void check_unit(const description& desc, Rack& rack, std::string label);
	void size_queue(const description& desc, Rack& rack);
};

} // Config
} // StrangeIO;
#endif

