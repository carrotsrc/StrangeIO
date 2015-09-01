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
#ifndef LIBRARYLOADER_H
#define LIBRARYLOADER_H
#include "framework/fwcommon.hpp"

#define NATIVE_LINUX 1

#if NATIVE_LINUX
#include "framework/dynlib/LinuxDynamicLibrary.hpp"
#endif

namespace strangeio {

class LibraryLoader {
public:
	static std::unique_ptr<DynamicLibrary> load(std::string libpath);
};

}
#endif
