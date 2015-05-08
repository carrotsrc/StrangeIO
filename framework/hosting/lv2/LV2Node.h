#ifndef LV2NODE_H
#define LV2NODE_H

#include <lilv/lilv.h>
#include "framework/common.h"

namespace RackoonIO {
namespace Hosting {

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
	const char* asBlank();
	const char* asString();
	float asFloat();
	int asInt();
	bool asBool();

	const LilvNode* operator *() const {
		return node;
	}

private:
	
	LilvNode *node;
};


} // Hosting
} // RackoonIO
#endif
