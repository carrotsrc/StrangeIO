#ifndef __COMPONENT_HPP_1440410429__
#define __COMPONENT_HPP_1440410429__
#include "framework/fwcommon.hpp"

namespace StrangeIO {
namespace Component {

enum class UnitType {
	Mainliner, Passer, Combiner, Splitter, Dispatcher
};

enum class ComponentState {
	Active, Inactive
};

enum class CycleType {
	Warmup, Sync, Ac
};

enum class CycleState {
	Complete, Partial, Error
};

enum class ProfileMetric {
	Latency, Channel, Period, Fs, Drift
};

struct LineProfile {
	int	latency,
		channel,
		period,
		fs,
		drift;
};

}
}
#endif
