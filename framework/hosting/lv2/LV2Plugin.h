#ifndef LV2PLUGIN_H
#define LV2PLUGIN_H
#include <lilv/lilv.h>
#include <cstdint>
#include "framework/common.h"
namespace RackoonIO {
namespace Hosting {

class LV2Plugin {
public:
	LV2Plugin(const LilvPlugin*);
	const LilvPlugin* getPlugin();

	std::string getName();
	uint32_t getNumPorts();
	std::string getUri();
private:
	const LilvPlugin* plugin;
};


} // Hosting
} // RackoonIO
#endif
