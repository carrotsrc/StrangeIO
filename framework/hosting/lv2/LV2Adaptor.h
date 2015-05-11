#ifndef LV2ADAPTOR_H
#define LV2ADAPTOR_H
#include "framework/common.h"
#include "framework/rack/UnitConnectors.h"
#include "LV2Platform.h"

namespace RackoonIO {
namespace Hosting {

class LV2Adaptor {
public:
	LV2Adaptor();

	void setPlatform(const LV2Platform*);

	bool init();
	FeedState feed(PcmSample*);

protected:
	std::unique_ptr<LV2Plugin> mPlugin;
	void loadPlugin(std::string);

private:
	std::map<std::string, SeqJack> jacks;
	std::map<std::string, Plug> plugs;

	const LV2Platform *mPlatform;
};

} // Hosting
} // RackoonIO
#endif
