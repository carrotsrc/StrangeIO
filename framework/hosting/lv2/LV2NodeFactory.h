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
#ifndef LV2NODEFACTORY_H
#define LV2NODEFACTORY_H

#include <lilv/lilv.h>
#include "framework/common.h"
#include "framework/hosting/lv2/LV2Node.h"

namespace RackoonIO {
namespace Hosting {

class LV2NodeFactory {
public:
	LV2NodeFactory(LilvWorld*);
	std::unique_ptr<LV2Node> newUri(std::string) const;
	std::unique_ptr<LV2Node> newString(std::string) const;
	std::unique_ptr<LV2Node> newInt(int32_t) const;
	std::unique_ptr<LV2Node> newFloat(float) const;
	std::unique_ptr<LV2Node> newBool(bool) const;

	std::unique_ptr<LV2Node> pack(LilvNode*) const;
private:
	LilvWorld* world;
};


} // Hosting
} // RackoonIO
#endif
