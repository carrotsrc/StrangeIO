#ifndef WORKERPACKAGE_H
#define WORKERPACKAGE_H
namespace RackoonIO {
#include <functional>
class WorkerPackage {
public:
	std::function<void()> run;

	WorkerPackage(std::function<void()> cbRun) {
		run = cbRun;
	}
};
}
#endif
