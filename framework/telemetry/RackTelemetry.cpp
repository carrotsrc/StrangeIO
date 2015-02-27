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
#include "RackTelemetry.h"
#include "common.h"
#if RACK_METRICS

using namespace RackoonIO;
using namespace RackoonIO::Telemetry;
using namespace std::chrono;

RackTelemetry::RackTelemetry(Rack *obj) {
	rack = obj;
	controlDuration = microseconds(0);
}

void RackTelemetry::metricUnitCycle() {
	rack->cbmetricUnitCycle(
			std::bind(&RackTelemetry::onUnitCycleStart, this, std::placeholders::_1),
			std::bind(&RackTelemetry::onUnitCycleEnd, this, std::placeholders::_1)
	);
	unitCycle.total = 0;
	unitCycle.sumDelta =
	unitCycle.avgDelta =
	unitCycle.peakDelta = steady_clock::duration::zero();
	unitCycle.lowDelta = seconds(1);

}

void RackTelemetry::onUnitCycleStart(steady_clock::time_point time) {
	mutUnitCycle.lock();
	unitCycle.curStart = time;
	mutUnitCycle.unlock();
}

void RackTelemetry::onUnitCycleEnd(steady_clock::time_point time) {
	mutUnitCycle.lock();
	duration<double, micro> delta = duration_cast<microseconds>(time - unitCycle.curStart);
	
	/*
	 * don't worry about overflow checks - you'll effectively
	 * be checking the system hasn't run for longer 
	 * than the standard model age of the universe. The CPU
	 * would have burnt out before then.
	 */
	if(delta > unitCycle.peakDelta) {
		unitCycle.peakDelta = delta;
	}
	else
	if(delta < unitCycle.lowDelta)
		unitCycle.lowDelta = delta;

	unitCycle.total++;

	unitCycle.avgDelta = unitCycle.sumDelta/unitCycle.total;
	mutUnitCycle.unlock();
}

const RackMetricsUnitCycle *RackTelemetry::getMetrics(RackTelemetry::Metrics metric) {
	switch(metric) {
	case UnitCycle:
		return &unitCycle;

	default:
		return nullptr;
	}
}

#endif
