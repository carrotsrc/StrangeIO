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
#include "LV2NodeFactory.h"

using namespace strangeio::Hosting;

LV2NodeFactory::LV2NodeFactory(LilvWorld* w) {
	world = w;
}

std::unique_ptr<LV2Node> LV2NodeFactory::newUri(std::string uri) const {
	auto n = lilv_new_uri(world, uri.c_str());
	return pack(n);
}

std::unique_ptr<LV2Node> LV2NodeFactory::newString(std::string str) const {
	auto n = lilv_new_string(world, str.c_str());
	return pack(n);
}

std::unique_ptr<LV2Node> LV2NodeFactory::newInt(int32_t val) const {
	auto n = lilv_new_int(world, val);
	return pack(n);
}

std::unique_ptr<LV2Node> LV2NodeFactory::newFloat(float val) const {
	auto n = lilv_new_float(world, val);
	return pack(n);
}

std::unique_ptr<LV2Node> LV2NodeFactory::newBool(bool val) const {
	auto n = lilv_new_bool(world, val);
	return pack(n);
}

std::unique_ptr<LV2Node> LV2NodeFactory::pack(LilvNode* n) const {
	return std::unique_ptr<LV2Node>(new LV2Node(n));
}
