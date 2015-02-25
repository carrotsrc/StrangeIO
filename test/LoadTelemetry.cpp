#include "common.h"
#include "framework/telemetry/RackTelemetry.h"
int main( void )
{
#ifdef RACK_METRICS
	RackoonIO::Telemetry::RackTelemetry track(NULL);
	const RackoonIO::Telemetry::RackMetricsUnitCycle *t = track.getMetricsUnitCycle();
	cout << t->total;
#endif
	return 0;
}
