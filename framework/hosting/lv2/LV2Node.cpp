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

const char* LV2Node::asURI(const LilvNode *n) {
	return lilv_node_as_uri(n);
}


const char* LV2Node::asBlank() {
	return lilv_node_as_blank(node);
}

const char* LV2Node::asBlank(const LilvNode *n) {
	return lilv_node_as_blank(n);
}


const char* LV2Node::asString() {
	return lilv_node_as_string(node);
}

const char* LV2Node::asString(const LilvNode *n) {
	return lilv_node_as_string(n);
}


float LV2Node::asFloat() {
	return lilv_node_as_float(node);
}

float LV2Node::asFloat(const LilvNode *n) {
	return lilv_node_as_float(n);
}


int LV2Node::asInt() {
	return lilv_node_as_int(node);
}

int LV2Node::asInt(const LilvNode *n) {
	return lilv_node_as_int(n);
}


bool LV2Node::asBool() {
	return lilv_node_as_bool(node);
}

bool LV2Node::asBool(const LilvNode *n) {
	return lilv_node_as_bool(n);
}

void LV2Node::free(LilvNode *n) {
	lilv_node_free(n);
}
