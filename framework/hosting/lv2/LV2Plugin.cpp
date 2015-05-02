#include "LV2Plugin.h"
using namespace RackoonIO::Hosting;


LV2Plugin::LV2Plugin(const LilvPlugin *p) {
	plugin = p;

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

void LV2Plugin::profilePorts() {
	auto n = getNumPorts();
	//auto nodeIn = lilv_new_uri(
	for(uint32_t i = 0; i < n; i++) {
		auto port = lilv_plugin_get_port_by_index(plugin, i);
		auto pname = lilv_port_get_name(plugin, port);
		std::cout << lilv_node_as_string(pname) << endl;
	}
}
