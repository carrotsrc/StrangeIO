#include "framework/Telemetry.h"
#include "framework/common.h"
#include "framework/telemetry/RackTelemetry.h"
int main( void )
{
#if RACK_METRICS
	RackoonIO::Telemetry::RackTelemetry track(NULL);
	const RackoonIO::Telemetry::RackMetricsUnitCycle *t = track.getMetrics(RackoonIO::Telemetry::RackTelemetry::UnitCycle);
	std::cout << t->total;
#endif
	return 0;
}
