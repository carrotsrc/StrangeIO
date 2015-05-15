/* Copyright 2015 Charlie Fyvie-Gauld
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published 
 *  by the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "RackUnitGenericFactory.h"
#include <dlfcn.h>

using namespace RackoonIO;

void RackUnitGenericFactory::setDependencies(RackUnit *unit) {
	unit->setEventLoop(eventLoop);
	unit->setMessageFactory(messageFactory);
	unit->setCacheHandler(cacheHandler);

#if HOSTING_LV2
	Hosting::LV2Adaptor *lv2u = dynamic_cast<Hosting::LV2Adaptor*>(unit);
	if(lv2u) {
		lv2u->setPlatform(lv2Platform);
	}
#endif
}


void RackUnitGenericFactory::setEventLoop(EventLoop *loop) { 
	eventLoop = loop; 
}

void RackUnitGenericFactory::setCacheHandler(CacheHandler *handler) {
	cacheHandler = handler;
}

void RackUnitGenericFactory::setMessageFactory(GenericEventMessageFactory *factory) { 
	messageFactory = factory; 
}

std::unique_ptr<RackUnit> RackUnitGenericFactory::load(std::string target, std::string unit, std::string name) {
	RackUnit*(*sym)(void);
	auto handle = dlopen(target.c_str(), RTLD_NOW);

	if(handle == NULL)
		std::cerr << "Failed to load library" << std::endl;
	else
		std::cout << "Loaded library successfully" << std::endl;

	std::string symbol = unit+std::string("Build");
	sym =(RackUnit*(*)(void)) dlsym(handle, symbol.c_str());
	if(sym == NULL)
		std::cerr << "Failed to find symbol" << std::endl;
	else
		std::cout << "Found symbol successfully" << std::endl;

	auto u = (*sym)();
	u->setName(name);
	return std::unique_ptr<RackUnit>(u);
}

#if HOSTING_LV2
void RackUnitGenericFactory::setLV2Platform(Hosting::LV2Platform *platform) { 
	lv2Platform = platform; 
}
#endif
