#include "LV2Node.h"

using namespace RackoonIO::Hosting;

LV2Node::LV2Node(LilvNode *n) {
	node = n;
}

LV2Node::~LV2Node() {
	lilv_node_free(node);
	node = nullptr;
}

const LilvNode *LV2Node::get() {
	return node;
}

bool LV2Node::is(NodeType type) {
	switch(type) {
	case URI: return lilv_node_is_uri(node);
	case Blank: return lilv_node_is_blank(node);
	case String: return lilv_node_is_string(node);
	case Float: return lilv_node_is_float(node);
	case Int: return lilv_node_is_int(node);
	case Bool: return lilv_node_is_bool(node);
	case Literal: return lilv_node_is_literal(node);
	default: return false;
	}
}

const char* LV2Node::asURI() {
	return lilv_node_as_uri(node);
}

const char* LV2Node::asBlank() {
	return lilv_node_as_blank(node);
}

const char* LV2Node::asString() {
	return lilv_node_as_string(node);
}

float LV2Node::asFloat() {
	return lilv_node_as_float(node);
}

int LV2Node::asInt() {
	return lilv_node_as_int(node);
}

bool LV2Node::asBool() {
	return lilv_node_as_bool(node);
}
