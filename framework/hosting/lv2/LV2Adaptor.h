#ifndef LV2ADAPTOR_H
#define LV2ADAPTOR_H
#include "framework/common.h"
#include "framework/rack/RackUnit.h"
#include "LV2Platform.h"

namespace RackoonIO {
namespace Hosting {
/**
 * A general purpose adaptor for 
 */
class LV2Adaptor : public RackUnit {
public:
	LV2Adaptor(std::string);
	void setPlatform(const LV2Platform*);

	virtual RackState init() = 0;
	virtual void setConfig(std::string, std::string) = 0;
	virtual FeedState feed(Jack*) = 0;
	virtual RackState cycle() = 0;
	virtual void block(Jack*) = 0;


protected:
	std::unique_ptr<LV2Plugin> mPlugin;

	void loadPlugin(std::string, bool = false);
	virtual void feedPlugin(Jack*) = 0;
	bool initPlugin();

private:

	const LV2Platform *mPlatform;
};

} // Hosting
} // RackoonIO
#endif
