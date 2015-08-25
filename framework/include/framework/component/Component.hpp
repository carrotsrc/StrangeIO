#ifndef __COMPONENT_HPP_1440410429__
#define __COMPONENT_HPP_1440410429__
#include "framework/fwcommon.hpp"

namespace StrangeIO {
namespace Component {

enum class UnitType {
	Mainliner, Passer, Combiner, Splitter, Dispatcher
};

enum class ComponentState {
	Inactive, Active
};

enum class CycleType {
	Warmup, Sync, Ac
};

enum class CycleState {
	Complete, Partial, Error
};

enum class ProfileMetric {
	Latency, Channels, Period, Fs, Drift
};

struct Profile {
	int	latency,
		channels,
		period,
		fs,
		drift;
};

}
}
#endif
