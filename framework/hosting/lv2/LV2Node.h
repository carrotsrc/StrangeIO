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

private:
	
	LilvNode *node;
};


} // Hosting
} // RackoonIO
#endif
