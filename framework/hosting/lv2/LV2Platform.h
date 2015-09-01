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
#ifndef LV2_H
#define LV2_H
#include <lilv/lilv.h>

#include "framework/common.h"
#include "LV2Plugin.h"

namespace strangeio {

namespace Hosting {

class LV2Platform {
	public:
		LV2Platform();
		~LV2Platform();

		const LilvPlugins* getPlugins();
		std::unique_ptr<LV2Plugin> getPlugin(std::string);
	private:

	LilvWorld* world;
	LV2NodeFactory *factory;

};

} // Hosting
} // StrangeIO
#endif
