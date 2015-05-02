#ifndef LV2PLUGIN_H
#define LV2PLUGIN_H
#include <lilv/lilv.h>
#include "framework/common.h"
#include "framework/hosting/lv2/LV2NodeFactory.h"
namespace RackoonIO {
namespace Hosting {

class LV2Plugin {
public:
	LV2Plugin(const LilvPlugin*, const LV2NodeFactory*);
	const LilvPlugin* getPlugin();

	std::string getName();
	uint32_t getNumPorts();
	std::string getUri();

protected:
	const LV2NodeFactory *nodeFactory;

private:
	const LilvPlugin* plugin;
	std::vector<const LilvPort*> input;
	std::vector<const LilvPort*> output;

	void profilePorts();
};


} // Hosting
} // RackoonIO
#endif
