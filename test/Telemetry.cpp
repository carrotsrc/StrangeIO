#include "framework/Telemetry.h"
#include "framework/common.h"
#include "framework/telemetry/RackTelemetry.h"
int main( void )
{
#if RACK_METRICS
	StrangeIO::Telemetry::RackTelemetry track(NULL);
	const StrangeIO::Telemetry::RackMetricsUnitCycle *t = track.getMetrics(StrangeIO::Telemetry::RackTelemetry::UnitCycle);
	std::cout << t->total;
#endif
	return 0;
}
