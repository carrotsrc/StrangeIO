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

	template<typename T>
	T as(NodeType type) {
		switch(type) {
		case URI: return lilv_node_as_uri(node);
		case Blank: return lilv_node_as_blank(node);
		case String: return lilv_node_as_string(node);
		case Float: return lilv_node_as_float(node);
		case Int: return lilv_node_as_int(node);
		case Bool: return lilv_node_as_bool(node);
		default: return nullptr;
		}
	}

private:
	LilvNode *node;
};


} // Hosting
} // RackoonIO
#endif
