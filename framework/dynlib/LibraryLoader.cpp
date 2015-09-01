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
#include <fstream>
#include "framework/dynlib/LibraryLoader.hpp"
using namespace strangeio;
std::unique_ptr<DynamicLibrary> LibraryLoader::load(std::string libpath) {
	std::ifstream f(libpath.c_str());
	auto good = f.good();
	f.close();
	
	if(!good) return nullptr;
	

	try {
		auto lib = std::unique_ptr<DynamicLibrary>(new DynamicLibrary(libpath));
		return lib;
	} catch(std::exception& e) {
		return nullptr;
	}
}
