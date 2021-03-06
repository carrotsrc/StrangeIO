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
#include <fstream>
#include <memory>

#include "framework/fwcommon.hpp"



namespace strangeio {

class library {
public:
	library(std::string path) :
	m_handle(nullptr) {
		m_handle = dlopen(path.c_str(), RTLD_NOW|RTLD_GLOBAL);
		if(m_handle == NULL) {
			std::cerr << "Dynamic link error: Failed to load " << path << std::endl;
			std::cerr << "Error: " << dlerror() << std::endl;
			throw;
		}
	}
	
	~library() {
		if(m_handle) dlclose(m_handle);
	}

	template<typename T>
	T load_symbol(std::string sym) {

		if(m_handle == nullptr) return nullptr;
		
		void *symbol = dlsym(m_handle, sym.c_str());
		if(symbol == NULL) {
			std::cerr << "Symbol resolve error: " << 
			dlerror() << std::endl;
			return nullptr;
		}

		return (T)(symbol);
	}

	void close() {

		if(!m_handle) return;

		dlclose(m_handle);
	}

	static std::unique_ptr<library> load(std::string libpath) {
		std::ifstream f(libpath.c_str());
		auto good = f.good();
		f.close();
		
		if(!good) {
			std::cerr << "Dynamic link error: " << 
			libpath << " does not exist" << std::endl;
			return nullptr;
		}
		

		try {
			auto lib = std::unique_ptr<library>(new library(libpath));
			return lib;
		} catch(std::exception& e) {
			return nullptr;
		}
	}

private:
	void *m_handle;
};

using library_uptr = std::unique_ptr<library>;

} // StrangeIO
#endif
