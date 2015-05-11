#include "LV2Plugin.h"
using namespace RackoonIO::Hosting;


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

void LV2Plugin::init() {
	if(!inst)
		return;

	lilv_instance_activate(inst);
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
			input.insert(std::pair<std::string, const LilvPort *>(std::string(pname), port));
		} else
		if(lilv_port_is_a(plugin, port, **nodeOut)) {
			output.insert(std::pair<std::string, const LilvPort *>(std::string(pname), port));
		}
	}
}

bool LV2Plugin::instantiate() {
	if(lilv_plugin_instantiate(plugin, mFs, NULL) == NULL)
		return false;

	return true;
}
