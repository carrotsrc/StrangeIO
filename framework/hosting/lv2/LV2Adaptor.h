#ifndef LV2ADAPTOR_H
#define LV2ADAPTOR_H
#include "framework/common.h"
#include "framework/rack/RackUnit.h"
#include "LV2Platform.h"

namespace RackoonIO {
namespace Hosting {

class LV2Adaptor : public RackUnit {
public:
	void setPlatform(const LV2Platform*);

	virtual RackState init();
	virtual void setConfig(std::string, std::string);
	virtual FeedState feed(Jack*);
	virtual RackState cycle();
	virtual void block(Jack*);


protected:
	std::unique_ptr<LV2Plugin> mPlugin;

	void loadPlugin(std::string);
	void feedPlugin(Jack*);
	bool initPlugin();

private:

	const LV2Platform *mPlatform;
};

} // Hosting
} // RackoonIO
#endif
