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
#include "framework/alias.hpp"
#include "framework/config/assembler.hpp"
#include "framework/midi/midi_handler.hpp"
#include "framework/thread/scheduled.hpp"

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
	
#ifdef __linux__
	assemble_schpolicy(desc, sys);
#endif
	
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

		u->set_led_utility(sys.get_midi_handler());
		
		if(u->controllable()) {
			assemble_bindings(desc, sys, *u);
		}
		
		assemble_leds(desc, sys, *u);

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

void assembler::assemble_leds(const description& desc, component::rack& sys, component::unit& u) {
	auto handler = sys.get_midi_handler();
	auto& leds = unit_description(desc, u.ulabel()).leds;
	auto led_map =  u.midi_leds();

	for(auto& led : leds) {
		auto exp = led_map.find(led.state);
		if(exp != led_map.end()) {
			auto mode = siomid::led_mode::steady;
			if(led.mode == "blink")
				mode = siomid::led_mode::blink;
			else if(led.mode == "off")
				mode = siomid::led_mode::off;
			auto label = u.ulabel();
			handler->add_led(label, exp->second, led.device, led.code, led.value, mode);
		}
	}

}

void assembler::assemble_devices(const description& desc, rack& sys) {
	auto midi_handler = sys.get_midi_handler();
	std::string d;
	for(auto device : desc.midi.controllers) {
		midi_handler->add_device(device.port, device.label);
		d = device.label;
		auto dev = midi_handler->get(device.label);
		dev->set_led_on(device.led_on);
		dev->set_led_off(device.led_off);
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

#ifdef __linux__
void assembler::assemble_schpolicy(const description& desc, rack& sys) {
	siothr::sched_desc sd;
	sd.priority = desc.system.linux_sys.priority;
	sd.policy = SCHED_OTHER;
	
	if(desc.system.linux_sys.policy == "SCHED_FIFO") {
		sd.policy = SCHED_FIFO;
	} else if(desc.system.linux_sys.policy == "SCHED_RR") {
		sd.policy = SCHED_RR;
	} else if(desc.system.linux_sys.policy == "SCHED_OTHER") {
		sd.policy = SCHED_OTHER;
	} else if(desc.system.linux_sys.policy == "SCHED_BATCH") {
#ifdef __USE_GNU
		sd.policy = SCHED_BATCH;
#endif
	} else if(desc.system.linux_sys.policy == "SCHED_IDLE") {
#ifdef __USE_GNU
		sd.policy = SCHED_IDLE;
#endif
	}
	
	sys.assign_schpolicy(sd);
}
#endif
