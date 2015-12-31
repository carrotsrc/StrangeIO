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
#include <memory>
#include "LV2Platform.h"
using namespace strangeio::Hosting;

LV2Platform::LV2Platform() {
	world = lilv_world_new();

	if(world == NULL) {
		throw new std::bad_alloc();
	}

	lilv_world_load_all(world);
	factory = new LV2NodeFactory(world);
}


LV2Platform::~LV2Platform() {
	lilv_world_free(world);
}


const LilvPlugins* LV2Platform::getPlugins() {
	return lilv_world_get_all_plugins(world);
}


std::unique_ptr<LV2Plugin> LV2Platform::getPlugin(std::string uri) {
	auto plugins = getPlugins();
	const LilvPlugin *plugin;
	auto uriNode = lilv_new_uri(world, uri.c_str());

	if((plugin = lilv_plugins_get_by_uri(plugins, uriNode)) == NULL) {
		return nullptr;
	}

	if(!lilv_plugin_verify(plugin)) {
		return nullptr;
	}

	return std::unique_ptr<LV2Plugin>(new LV2Plugin(plugin, factory));
}
