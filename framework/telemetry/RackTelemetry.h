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

typedef struct {
	std::chrono::duration<double, micro> peakDelta;
	std::chrono::duration<double, micro> lowDelta;
	std::chrono::duration<double, micro> avgDelta; 
	std::chrono::duration<double, micro> sumDelta;
	std::chrono::steady_clock::time_point curDelta; 
	double total;
} RackMetricsUnitCycle;

class RackTelemetry
{
public:
	enum Metrics {
		UnitCycle
	};

	RackTelemetry(Rack*);
	void metricUnitCycle();

	const RackMetricsUnitCycle *getMetrics(RackTelemetry::Metrics);
protected:
	Rack *rack;
	RackMetricsUnitCycle unitCycle;
	std::mutex mutUnitCycle;

	std::chrono::duration<double, micro> controlDuration;

	void onUnitCycleStart(std::chrono::steady_clock::time_point);
	void onUnitCycleEnd(std::chrono::steady_clock::time_point);
};

} // Telemetry

} // RackoonIO

#endif // RACK_METRICS

#endif // RACKTELEMETRY_H
