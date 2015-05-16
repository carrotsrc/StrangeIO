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
#include "LV2Plugin.h"
using namespace RackoonIO::Hosting;

using port_map = std::map<std::string, LV2Port>;

LV2Plugin::LV2Plugin(const LilvPlugin *p, const LV2NodeFactory *f) {
	plugin = p;
	inst = nullptr;
	nodeFactory = f;

	mFs = 44100;
	profilePorts();
}
LV2Plugin::~LV2Plugin() {
	if(inst)
		lilv_instance_free(inst);
}

const LilvPlugin* LV2Plugin::getPlugin() {
	return plugin;
}

std::string LV2Plugin::getName() {
	return std::string(lilv_node_as_string(lilv_plugin_get_name(plugin)));
}

uint32_t LV2Plugin::getNumPorts() {
	return uint32_t(lilv_plugin_get_num_ports(plugin));
}

std::string LV2Plugin::getUri() {
	return std::string(lilv_node_as_string(lilv_plugin_get_uri(plugin)));
}

void LV2Plugin::setFs(double rate) {
	mFs = rate;
}

double LV2Plugin::getFs() {
	return mFs;
}

bool LV2Plugin::init() {
	if(!inst)
		return false;

	lilv_instance_activate(inst);
	return true;
}

void LV2Plugin::profilePorts() {
	auto n = getNumPorts();
	
	auto nodeIn = nodeFactory->newUri(LILV_URI_INPUT_PORT);
	auto nodeOut = nodeFactory->newUri(LILV_URI_OUTPUT_PORT);
	auto nodeControl = nodeFactory->newUri(LILV_URI_CONTROL_PORT);

	for(uint32_t i = 0; i < n; i++) {
		auto port = lilv_plugin_get_port_by_index(plugin, i);
		
		auto pname = LV2Node::asString(lilv_port_get_name(plugin, port));
		if(lilv_port_is_a(plugin, port, **nodeIn)) {
			auto p = LV2Port { std::string(pname), LV2Port::Input, i, port };
			ports.insert(std::pair<std::string, LV2Port>(std::string(pname), p));
		} else
		if(lilv_port_is_a(plugin, port, **nodeOut)) {
			auto p = LV2Port { std::string(pname), LV2Port::Output, i, port };
			ports.insert(std::pair<std::string, LV2Port>(std::string(pname), p));
		}
	}
}

bool LV2Plugin::instantiate() {
	if((inst = lilv_plugin_instantiate(plugin, mFs, NULL)) == NULL)
		return false;

	return true;
}


void LV2Plugin::connectPort(std::string pname, void* data) {
	const auto port = getPort(pname);
	if(port == nullptr)
		return;
	lilv_instance_connect_port(inst, port->index, data);
}

void LV2Plugin::connectPort(uint32_t index, void* data) {
	lilv_instance_connect_port(inst, index, data);
}

const LV2Port *LV2Plugin::getPort(std::string name) {
	try {
		const auto p = &(ports.at(name));
		return p;
	} catch(const std::out_of_range& oor) {
		return nullptr;
	}
}

const LV2Port *LV2Plugin::getPort(uint32_t index) {
	port_map::const_iterator it;

	for(it = ports.begin(); it != ports.end(); it++) {
		if(it->second.index == index) {
			return &(it->second);
		}
	}

	return nullptr;
}

std::vector<const LV2Port*> LV2Plugin::getPortsOfType(LV2Port::PortType type) {
	std::vector<const LV2Port*> list;
	port_map::const_iterator it;

	for(it = ports.begin(); it != ports.end(); it++) {
		if(it->second.type == type)
			list.push_back(&(it->second));
	}

	return list;
}

void LV2Plugin::run(uint32_t num) {
	lilv_instance_run(inst, num);
}
