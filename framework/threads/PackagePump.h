#ifndef PACKAGEPUMP_H
#define PACKAGEPUMP_H
#include "common.h"
#include "WorkerPackage.h"

namespace RackoonIO {


class PackagePump {
public:
	PackagePump(std::mutex *mutex, std::condition_variable *cv);

	void addPackage(std::unique_ptr<WorkerPackage>);
	std::unique_ptr<WorkerPackage> getPackage();
private:
	std::condition_variable *mConditional;
	std::mutex *mSharedMutex, mQueueMutex;
	std::vector<std::unique_ptr<WorkerPackage>> mQueue;
};

}
#endif
