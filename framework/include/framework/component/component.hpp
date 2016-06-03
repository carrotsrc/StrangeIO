#ifndef __COMPONENT_HPP_1440410429__
#define __COMPONENT_HPP_1440410429__
#include <chrono>
#include "framework/fwcommon.hpp"
namespace strangeio {
namespace component {

	/**
	 * The different types of unit that can be plugged in. The type provides
	 * a general description of what the unit does, and provide specialised
	 * behavior
	 */
enum class unit_type {
	mainline, ///< These are the initial generator of samples 
	step, ///< A unit that provides a general step in the audio pipeline 
	combine, ///< A unit that takes multiple lines and combines them together
	split, ///< A unit that takes a single line and splits them into many
	dispatch ///< A unit that is at the end of the line, sending samples out
};

/**
 * State of the component
 */
enum class component_state {
	inactive,active
};

/**
 * State of an entire line of components
 */
enum class line_state {
	inactive, active, flushing
};

enum class cycle_type {
	warmup, sync, ac
};

/**
 * The state of the cycle at the end of the callstack
 */
enum class cycle_state {
	complete, ///< The cycle reached the dispatch unit
	partial, ///< Only one of many lines have performed their cycle
	error, ///< One of the units errored out
	empty ///< There is nothing to cycle
};

/**
 * Various measurements that are used for the profile of streams, units, lines
 * and the entire rack.
 * 
 * Note: Many of these things are being moved out into the stream profiles
 */
enum class profile_metric {
	latency, ///< The number of periods of measured latency
	channels, ///< Number of channels
	period, ///< The number of frames in a unit of processing
	fs, ///< The sample rate
	drift, ///< The ratio of speed change
	state, ///< The state
	bpm, ///< Beats per minute
	fill ///< How full a buffer is
};


// Syncing
typedef int sync_flag;

/**
 * Flags to specify types of syncs to perform
 */
enum class sync_flags {
	none = 0, ///< Null for testing
	source = 1, ///< Specify that the unit is the source of the upstream sync
	sync_duration = 2, ///< The time it takes to perform a sync
	glob_sync = 4, ///< Perform a global sync when the global state changes
	upstream = 8, ///< Perform an upstream sync
    downstream = 16, ///< Perform a downstream sync
};

using profile_duration = std::chrono::microseconds;

/**
 * The measured profile or state profile
 */
struct sync_profile {
	int	latency, ///< The measured latency in terms of periods
		channels, ///< The number of audio channels
		period, ///< The measured number of frames per unit of processing
		fs, ///< The sample rate
		bpm; ///< The beats per minute

	float drift; ///< The ratio of speed change

	int jumps, ///< The number of units
		state, ///< The current state
		fill; ///< How full a buffer is
};

}
}
#endif
