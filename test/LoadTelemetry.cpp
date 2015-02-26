#include "common.h"
#include "framework/telemetry/RackTelemetry.h"
int main( void )
{
#ifdef RACK_METRICS
	RackoonIO::Telemetry::RackTelemetry track(NULL);
	const RackoonIO::Telemetry::RackMetricsUnitCycle *t = track.getMetrics(RackoonIO::Telemetry::RackTelemetry::UnitCycle);
	cout << t->total;
#endif
	return 0;
}
