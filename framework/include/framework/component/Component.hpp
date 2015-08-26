#ifndef __COMPONENT_HPP_1440410429__
#define __COMPONENT_HPP_1440410429__
#include <chrono>
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


// Syncing
typedef int SyncFlag;

enum class SyncFlags {
	Source = 1,
};

using ProfileDuration = std::chrono::microseconds;

struct Profile {
	int	latency, channels,
		period, fs;

	float drift;

	int jumps;
};

}
}
#endif