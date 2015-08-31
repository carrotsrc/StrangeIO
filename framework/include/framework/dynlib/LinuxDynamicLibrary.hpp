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
#ifndef DYNAMICLIBRARY_H
#define DYNAMICLIBRARY_H
#include <dlfcn.h>
#include <iostream>
#include <memory>

#include "framework/fwcommon.hpp"



namespace StrangeIO {

class DynamicLibrary {
public:
	DynamicLibrary(std::string path) :
	m_handle(nullptr) {
		m_handle = dlopen(path.c_str(), RTLD_NOW|RTLD_GLOBAL);
		if(m_handle == NULL) {
			std::cerr << "DynamicLibrary: Failed to load " << path << std::endl;
			throw;
		}
	}
	
	~DynamicLibrary() {
		if(m_handle) dlclose(m_handle);
	}

	template<typename T>
	T load_symbol(std::string sym) {
		void *symbol = dlsym(m_handle, sym.c_str());
		if(symbol == NULL) {
			return nullptr;
		}

		return (T)(symbol);
	}

	void close() {
		dlclose(m_handle);
	}

private:
	void *m_handle;
};

} // StrangeIO
#endif
