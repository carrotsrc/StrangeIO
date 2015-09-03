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
#include "framework/config/assembler.hpp"
#include "framework/midi/midi_handler.hpp"

using namespace strangeio;
using namespace strangeio::config;
using namespace strangeio::component;

assembler::assembler(std::unique_ptr<unit_factory> factory) {
	m_factory = std::move(factory);
}

void assembler::assemble(const description& desc, rack& sys) {
	size_queue(desc,sys);
	assemble_mainlines(desc, sys);
	assemble_devices(desc, sys);
	assemble_daisychains(desc, sys);
}

void assembler::assemble_mainlines(const description& desc, rack& sys) {
	for(auto mainline : desc.setup.mainlines) {
		sys.addMainline(mainline);
	}
}

void assembler::assemble_daisychains(const description& desc, rack& sys) {
	for(auto link : desc.setup.daisychains) {
		check_unit(desc, sys, link.to);
		sys.connectUnits(link.from, link.plug, link.to, link.jack);
	}
}

const unit_desc& assembler::unit_description(const description& desc, std::string label) {
	for(const auto& u : desc.setup.units) {
		if(u.label == label) {
			return u;
		}
	}

	throw;
}

std::unique_ptr<RackUnit> assembler::assemble_unit(std::string model, std::string label, std::string target) {
	return m_factory->load(model, label, target);

}

void assembler::check_unit(const description& desc, rack& sys, std::string label) {
	auto& ud = unit_description(desc, label);
	if(!sys.hasUnit(ud.label)) {
		auto u = assemble_unit(ud.unit, ud.label, ud.library);
		
		for(const auto& config : ud.configs) {
			u->set_configuration(config.type, config.value);
		}

		if(u->midiControllable()) {
			assemble_bindings(desc, sys, *u);
		}

		u->setRackQueue(sys.getRackQueue());

		sys.addUnit(std::move(u));
	}
}

void assembler::assemble_bindings(const description& desc, rack& sys, RackUnit& unit) {
	auto midi = sys.midi_handlers();
	auto& bindings = unit_description(desc, unit.getName()).bindings;
	auto exports = unit.midiExportedMethods();

	for(auto& binding : bindings) {
		auto exp = exports.find(binding.name);
		if(exp != exports.end()) {
			midi.addBinding(binding.module, binding.code, exp->second);
		}
	}
}

void assembler::assemble_devices(const description& desc, rack& sys) {
	auto& midi_handler = sys.midi_handlers();
	for(auto device : desc.midi.controllers) {
		midi_handler.addModule(device.port, device.label);
	}
}

void assembler::size_queue(const description& desc, rack& sys) {
	auto queue = sys.getRackQueue();
	if(!queue) return;

	queue->setSize(desc.system.threads.num_workers);
}
