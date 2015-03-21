#ifndef PACKAGEPUMP_H
#define PACKAGEPUMP_H
#include "common.h"
#include "WorkerPackage.h"

namespace RackoonIO {


class PackagePump {
public:
	PackagePump();

	void addPackage(std::unique_ptr<WorkerPackage>);
	std::unique_ptr<WorkerPackage> nextPackage();

	int getLoad();
private:
	std::vector<std::unique_ptr<WorkerPackage>> mQueue;
	std::mutex mQueueMutex;

};

}
#endif
