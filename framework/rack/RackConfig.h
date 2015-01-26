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
	RACK,
};

typedef struct {
	string plug;
	string jack;
	string unit;
	string name;
} ConfigConnection;

}
#endif
