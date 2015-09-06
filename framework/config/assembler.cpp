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
assembler::assembler()
	: m_factory(nullptr)
{}
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
		sys.add_mainline(mainline);
	}
}

void assembler::assemble_daisychains(const description& desc, rack& sys) {
	for(auto link : desc.setup.daisychains) {
		check_unit(desc, sys, link.to);
		if(link.from == "rack") {
			sys.connect_mainline(link.plug, link.to);
		} else {
			sys.connect_units(link.from, link.plug, link.to, link.jack);
		}
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

unit_uptr assembler::assemble_unit(std::string model, std::string label, std::string target) {
	return nullptr;
}

void assembler::check_unit(const description& desc, rack& sys, std::string label) {
	auto& ud = unit_description(desc, label);
	if(!sys.has_unit(ud.label)) {

		auto u = m_factory->load(ud.unit, ud.label, ud.library);
		
		if(!u) { return; }

		if(u->controllable()) {
			assemble_bindings(desc, sys, *u);
		}

		for(const auto& config : ud.configs) {
			auto& k = config.type;
			auto& v = config.value;
			u->set_configuration(k, v);
		}

		sys.add_unit(std::move(u));
	}
}

void assembler::assemble_bindings(const description& desc, rack& sys, unit& u) {
	auto handler = sys.get_midi_handler();
	auto& bindings = unit_description(desc, u.ulabel()).bindings;
	auto exports = u.midi_handlers();

	for(auto& binding : bindings) {
		auto exp = exports.find(binding.name);
		if(exp != exports.end()) {
			handler->add_binding(binding.module, binding.code, exp->second);
		}
	}
}

void assembler::assemble_devices(const description& desc, rack& sys) {
	auto midi_handler = sys.get_midi_handler();
	for(auto device : desc.midi.controllers) {
		midi_handler->add_device(device.port, device.label);
	}
}

void assembler::size_queue(const description& desc, rack& sys) {
	auto queue = sys.get_queue_utility();
	if(!queue) return;

	queue->set_size(desc.system.threads.num_workers);
}

void assembler::set_builder(std::unique_ptr<unit_factory> builder) {
	m_factory = std::move(builder);
}
