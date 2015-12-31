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
#ifndef LV2PLUGIN_H
#define LV2PLUGIN_H
#include <lilv/lilv.h>
#include "framework/common.h"
#include "framework/hosting/lv2/LV2NodeFactory.h"
#include "framework/hosting/lv2/LV2Port.h"
namespace strangeio {
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
} // StrangeIO
#endif
