#ifndef __COMPONENT_HPP_1440410429__
#define __COMPONENT_HPP_1440410429__
#include "framework/fwcommon.hpp"

namespace StrangeIO {
namespace Component {

enum UnitType {
	Mainliner, Passer, Combiner, Splitter, Dispatcher
};

enum ComponentState {
	Active, Inactive
};

enum MetricType {
	Latency, Channel, Period
}

}
}
#endif
