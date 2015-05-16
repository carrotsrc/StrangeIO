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
#ifndef LV2NODE_H
#define LV2NODE_H

#include <lilv/lilv.h>
#include "framework/common.h"

namespace RackoonIO {
namespace Hosting {

using NodeUri = const char*;
using NodeBlank = const char*;
using NodeLiteral = const char*;
using NodeString = const char*;
using NodeFloat = float;
using NodeInt = int;
using NodeBool = bool;

class LV2Node {
public:
	enum NodeType {
		URI,
		Blank,
		Literal,
		String,
		Float,
		Int,
		Bool
	};

	LV2Node(LilvNode*);
	~LV2Node();
	const LilvNode* get();

	bool is(NodeType);

	const char* asURI();
	static const char* asURI(const LilvNode*);

	const char* asBlank();
	static const char* asBlank(const LilvNode*);

	const char* asString();
	static const char* asString(const LilvNode*);

	float asFloat();
	static float asFloat(const LilvNode*);

	int asInt();
	static int asInt(const LilvNode*);

	bool asBool();
	static bool asBool(const LilvNode*);

	const LilvNode* operator *() const {
		return node;
	}

	static void free(LilvNode*);

private:
	
	LilvNode *node;
};


} // Hosting
} // RackoonIO
#endif
