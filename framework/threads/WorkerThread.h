/* Copyright 2015 Charlie Fyvie-Gauld
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published 
 *  by the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H
#include "common.h"
#include "PackagePump.h"
#include "WorkerPackage.h"
namespace RackoonIO {

/** This class acting as an interface for a worker thread
 *
 * The class encapsulates a thread object and exposes
 * an interface for handing work packages to the thread
 * for processing a task.
 */
class WorkerThread {
	bool mRunning; ///< Toggled when the thread is running
	bool mLoaded; ///< Toggled when the thread is running

	std::thread mWorker; ///< Pointer to the thread object
	unique_ptr<WorkerPackage> current; ///< The current WorkPackage
	//std::chrono::microseconds uSleep; ///< The microsecond sleep between checks
	PackagePump *mPump;
	std::condition_variable mCondition, *mReadyCondition;
	std::mutex mMutex;

	/** The internal threaded method for processing WorkerPackage tasks */
	void process();

	std::mutex pkg_lock; ///< Task lock
public:
	/** Instantiate the thread
	 * 
	 * @param autoStart Toggle whether the thread immediately starts
	 */
	WorkerThread(std::condition_variable *cv);

	/** Start the thread running
	 */
	void start();

	/** Stop the thread
	 */
	void stop();

	/** Assign a WorkerPackage task to the thread
	 *
	 * @param pkg A unique_ptr to the WorkPacakge. Thread takes ownership.
	 * @return true on successful transfer; otherwise false
	 */
	bool assignPackage(unique_ptr<WorkerPackage> pkg, bool unlock = true);

	bool isRunning();
	bool isLoaded();
	bool isWaiting();
	void notify();
};

}
#endif
