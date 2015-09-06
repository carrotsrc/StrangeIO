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
	assembler();
	assembler(std::unique_ptr<unit_factory>);
	
	void set_builder(std::unique_ptr<unit_factory> builder);
	
	void assemble(const description& desc, component::rack& sys);

	std::unique_ptr<component::unit> assemble_unit(std::string model, std::string label, std::string target);

private:
	std::unique_ptr<unit_factory> m_factory;

	void assemble_mainlines(const description& desc, component::rack& sys);
	void assemble_daisychains(const description& desc, component::rack& sys);
	void assemble_bindings(const description& desc, component::rack& sys, component::unit& u);
	void assemble_devices(const description& desc, component::rack& sys);

	const unit_desc& unit_description(const description& desc, std::string label);
	void check_unit(const description& desc, component::rack& sys, std::string label);
	void size_queue(const description& desc, component::rack& sys);
};

} // Config
} // StrangeIO;
#endif

