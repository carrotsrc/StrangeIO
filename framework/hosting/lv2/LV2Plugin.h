#ifndef LV2PLUGIN_H
#define LV2PLUGIN_H
#include <lilv/lilv.h>
#include "framework/common.h"
#include "framework/hosting/lv2/LV2NodeFactory.h"
#include "framework/hosting/lv2/LV2Port.h"
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
	bool init();
	void run(uint32_t);

	void connectPort(std::string, void*);
	void connectPort(uint32_t, void*);

	const LV2Port *getPort(std::string);
	const LV2Port *getPort(uint32_t);

	std::vector<const LV2Port*> getPortsOfType(LV2Port::PortType);

protected:
	const LV2NodeFactory *nodeFactory;
	double mFs;

private:
	const LilvPlugin* plugin;
	LilvInstance* inst;
	std::map<std::string, struct LV2Port> ports;

	void profilePorts();
};


} // Hosting
} // RackoonIO
#endif
