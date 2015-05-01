#include "LV2Plugin.h"
using namespace RackoonIO::Hosting;


LV2Plugin::LV2Plugin(const LilvPlugin* p) {
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
