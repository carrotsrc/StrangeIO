#ifndef __RACK_HPP_1440421209__
#define __RACK_HPP_1440421209__
#include "framework/component/Component.hpp"
#include <map>

namespace StrangeIO {
namespace Component {

class Rack {
public:
	Rack();
	~Rack();
	void register_metric(ProfileMetric type, int value);

protected:

private:
	std::map<ProfileMetric, int> m_global_metrics;
};

} // Component
} // StrangeIO
#endif

