#ifndef __COMPONENT_HPP_1440410429__
#define __COMPONENT_HPP_1440410429__
#include <chrono>
#include "framework/fwcommon.hpp"
namespace strangeio {
namespace component {

enum class unit_type {
	mainline, step, combine, split, dispatch
};

enum class component_state {
	inactive, active
};

enum class line_state {
	inactive, active
};

enum class cycle_type {
	warmup, sync, ac
};

enum class cycle_state {
	complete, partial, error, empty
};

enum class profile_metric {
	latency, channels, period, fs, drift, state, bpm, fill
};


// Syncing
typedef int sync_flag;

enum class sync_flags {
	none = 0,
	source = 1,
	sync_duration = 2,
	glob_sync = 4,
	upstream = 8,
};

using profile_duration = std::chrono::microseconds;

struct sync_profile {
	int	latency,
		channels,
		period,
		fs,
		bpm;

	float drift;

	int jumps,
		state,
		fill;
};

}
}
#endif
