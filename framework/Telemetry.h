// setup telemetry here
#define RACK_METRICS 1
#if RACK_METRICS
	#define RACK_TELEMETRY(func, ...) {if(func){ func(__VA_ARGS__); }}
#else
	#define RACK_TELEMETRY(func, ...) {}
#endif
