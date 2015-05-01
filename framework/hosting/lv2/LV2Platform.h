#ifndef LV2_H
#define LV2_H
#include "framework/common.h"
#include <lilv/lilv.h>

namespace RackoonIO {

namespace Hosting {

class LV2Platform {
	public:
		LV2Platform();
		~LV2Platform();

		const LilvPlugins* getPlugins();
		const LilvPlugin* getPlugin(std::string);
	private:

		LilvWorld *world;
};

} // Hosting
} // RackoonIO
#endif
