#ifndef RACKCONFIG_H
#define RACKCONFIG_H
namespace RackoonIO {
typedef struct {
	int cycle;

	int workers;
	int worker_us;
} ThreadsConfig;

typedef struct {
	ThreadsConfig threads;
} SystemConfig;

typedef struct {
	SystemConfig system;
} RackConfig;

enum RConfigArea {
	ROOT,
	SYSTEM,
	SYSTEM_THREADS
};

}
#endif
