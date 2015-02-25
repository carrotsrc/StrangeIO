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
using namespace RackoonIO;
using namespace RackoonIO::Telemetry;

RackTelemetry::RackTelemetry(Rack *obj) {
	rack = obj;
}

void RackTelemetry::metricUnitCycle() {
	rack->cbmetricUnitCycle(
			std::bind(&RackTelemetry::onUnitCycleStart, this, std::placeholders::_1),
			std::bind(&RackTelemetry::onUnitCycleEnd, this, std::placeholders::_1)
	);

}

void RackTelemetry::onUnitCycleStart(std::chrono::microseconds time) {
	unitCycle.curDelta = time;
}

void RackTelemetry::onUnitCycleEnd(std::chrono::microseconds time) {
	auto delta = time - unitCycle.curDelta;
	if(delta > unitCycle.peakDelta)
		unitCycle.peakDelta = delta;
}


