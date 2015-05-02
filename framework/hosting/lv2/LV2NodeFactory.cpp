#include "LV2NodeFactory.h"

using namespace RackoonIO::Hosting;

LV2NodeFactory::LV2NodeFactory(LilvWorld* w) {
	world = w;
}

std::unique_ptr<LV2Node> LV2NodeFactory::newUri(std::string uri) {
	auto n = lilv_new_uri(world, uri.c_str());
	return pack(n);
}

std::unique_ptr<LV2Node> LV2NodeFactory::newString(std::string str) {
	auto n = lilv_new_string(world, str.c_str());
	return pack(n);
}

std::unique_ptr<LV2Node> LV2NodeFactory::newInt(int32_t val) {
	auto n = lilv_new_int(world, val);
	return pack(n);
}

std::unique_ptr<LV2Node> LV2NodeFactory::newFloat(float val) {
	auto n = lilv_new_float(world, val);
	return pack(n);
}

std::unique_ptr<LV2Node> LV2NodeFactory::newBool(bool val) {
	auto n = lilv_new_bool(world, val);
	return pack(n);
}

std::unique_ptr<LV2Node> LV2NodeFactory::pack(LilvNode* n) {
	return std::unique_ptr<LV2Node>(new LV2Node(n));
}
