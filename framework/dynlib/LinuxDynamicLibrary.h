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

#include "framework/common.h"
#include "framework/rack/RackUnit.h"
#include <dlfcn.h>
namespace RackoonIO {
typedef RackUnit*(*DynamicUnitBuilder)(void);

class DynamicLibrary {
	void *handle;
public:
	DynamicLibrary(std::string path) {
		handle = dlopen(path.c_str(), RTLD_NOW);
		if(handle == NULL) {
			throw;
		}
	}

	template<typename T>
	T loadSymbol(std::string sym) {
		void *symbol = dlsym(handle, sym.c_str());
		if(symbol == NULL) {
			return nullptr;
		}

		return (T)(symbol);
	}
};

} // RackoonIO
#endif
