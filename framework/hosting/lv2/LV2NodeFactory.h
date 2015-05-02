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
	std::unique_ptr<LV2Node> newUri(std::string);
	std::unique_ptr<LV2Node> newString(std::string);
	std::unique_ptr<LV2Node> newInt(int32_t);
	std::unique_ptr<LV2Node> newFloat(float);
	std::unique_ptr<LV2Node> newBool(bool);

	std::unique_ptr<LV2Node> pack(LilvNode*);
private:
	LilvWorld* world;
};


} // Hosting
} // RackoonIO
#endif
