#include "common.h"
#include "framework/telemetry/RackTelemetry.h"
int main( void )
{
	RackoonIO::Telemetry::RackTelemetry track(NULL);
	const RackoonIO::Telemetry::RackMetricsUnitCycle *t = track.getMetricsUnitCycle();
	cout << t->total;
	return 0;
}
