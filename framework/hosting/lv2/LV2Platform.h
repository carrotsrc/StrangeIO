#ifndef LV2_H
#define LV2_H
#include <lilv/lilv.h>

#include "framework/common.h"
#include "LV2Plugin.h"

namespace RackoonIO {

namespace Hosting {

class LV2Platform {
	public:
		LV2Platform();
		~LV2Platform();

		const LilvPlugins* getPlugins();
		std::unique_ptr<LV2Plugin> getPlugin(std::string);
	private:

	LilvWorld* world;
};

} // Hosting
} // RackoonIO
#endif
