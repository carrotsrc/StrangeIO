#ifndef __COMPONENT_HPP_1440410429__
#define __COMPONENT_HPP_1440410429__
#include <chrono>
#include "framework/fwcommon.hpp"
namespace strangeio {
namespace component {

enum class unit_type {
	Mainliner, Stepper, Combiner, Splitter, Dispatcher
};

enum class component_state {
	Inactive, Active
};

enum class cycle_type {
	Warmup, Sync, Ac
};

enum class cycle_state {
	Complete, Partial, Error, Empty
};

enum class profile_metric {
	Latency, Channels, Period, Fs, Drift
};


// Syncing
typedef int sync_flag;

enum class sync_flags {
	Source = 1,
	SyncDuration = 2
};

using profile_duration = std::chrono::microseconds;

struct profile {
	int	latency, channels,
		period, fs;

	float drift;

	int jumps;
};

}
}
#endif
