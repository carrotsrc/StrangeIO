/*
 * Copyright (C) 2015  Charlie Fyvie-Gauld
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef RACKTELEMETRY_H
#define RACKTELEMETRY_H

#include "common.h"
#include "framework/rack/Rack.h"

#if RACK_METRICS

namespace RackoonIO {

namespace Telemetry {

/** A structure representing Unit Cycle metrics
 */
typedef struct {
	std::chrono::duration<double, micro> peakDelta; ///< The peak cycle time
	std::chrono::duration<double, micro> lowDelta; ///< The lowest cycle time
	std::chrono::duration<double, micro> avgDelta; ///< The average cycle time
	std::chrono::duration<double, micro> sumDelta; ///< The sum of all cycle times
	std::chrono::steady_clock::time_point curStart;  ///< The current starting time
	double total;
} RackMetricsUnitCycle;

/** A telemetry object for processing RackUnit metrics
 *
 * This can be used to monitor the Rack Unit and process any 
 * data sent back via callbacks.
 *
 * @note This telementry is toggled by RACK_METRICS
 */
class RackTelemetry
{
public:
	/** An enumeration of supported metrics */
	enum Metrics {
		UnitCycle ///< The metrics associated with Rack Unit cycles
	};

	/** Instantiate with a pointer to the target Rack 
	 *
	 * @param rack Pointer to the rack object
	 */
	RackTelemetry(Rack *rack);

	/** Toggle the UnitCyle metrics
	 */
	void metricUnitCycle();

	/** Get a form of metric
	 *
	 * @note Currently locked to UnitCycle
	 *
	 * Totally unsafe in with threads
	 *
	 * @param metric The type of metric to receive
	 * @return A const pointer to the metric object
	 */
	const RackMetricsUnitCycle *getMetrics(RackTelemetry::Metrics metric);
protected:
	Rack *rack; ///< Pointer to the target Rack object
	RackMetricsUnitCycle unitCycle; ///< Structure to store metrics
	std::mutex mutUnitCycle; ///> Storage for Unit Cycle measurements

	std::chrono::duration<double, micro> controlDuration; ///< A zero value control duration

	/** Callback function for signalling the start of a Unit Cycle
	 * 
	 * This is called to by the Rack to notify this monitor that the
	 * unit cycle has started at time specified
	 *
	 * @param time The start time
	 */
	void onUnitCycleStart(std::chrono::steady_clock::time_point time);

	/** Callback function for signalling the end of a Unit Cycle
	 * 
	 * This is called to by the Rack to notify this monitor that the
	 * unit cycle has ended at time specified
	 *
	 * @param time The end time
	 */
	void onUnitCycleEnd(std::chrono::steady_clock::time_point time);
};

} // Telemetry

} // RackoonIO

#endif // RACK_METRICS

#endif // RACKTELEMETRY_H
