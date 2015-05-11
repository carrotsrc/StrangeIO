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
	~LV2Plugin();
	const LilvPlugin* getPlugin();

	std::string getName();
	uint32_t getNumPorts();
	std::string getUri();

	void setFs(double);
	double getFs();

	bool instantiate();
	void init();

protected:
	const LV2NodeFactory *nodeFactory;
	double mFs;

private:
	const LilvPlugin* plugin;
	LilvInstance* inst;
	std::map<std::string, const LilvPort*> input;
	std::map<std::string, const LilvPort*> output;

	void profilePorts();
};


} // Hosting
} // RackoonIO
#endif
