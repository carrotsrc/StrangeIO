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
	bool busy; ///< Toggled when the thread is currently processing a task
	bool running; ///< Toggled when the thread is running

	std::thread *worker; ///< Pointer to the thread object
	unique_ptr<WorkerPackage> current; ///< The current WorkPackage
	//std::chrono::microseconds uSleep; ///< The microsecond sleep between checks
	PackagePump *mPump;
	std::condition_variable *mCondition;
	std::mutex *mSharedMutex;
	


	/** The internal threaded method for processing WorkerPackage tasks */
	void process();

	std::mutex pkg_lock; ///< Task lock
public:
	/** Instantiate the thread
	 * 
	 * @param autoStart Toggle whether the thread immediately starts
	 */
	WorkerThread(std::condition_variable *condition, std::mutex *mutex, PackagePump *pump);

	/** Start the thread running
	 */
	void start();

	/** Stop the thread
	 */
	void stop();

	/** Check if the thread is currently busy processing a task
	 *
	 * @return true if the thread is free; otherwise false if it is busy
	 */
	bool isBusy();

	/** Assign a WorkerPackage task to the thread
	 *
	 * @param pkg A unique_ptr to the WorkPacakge. Thread takes ownership.
	 * @return true on successful transfer; otherwise false
	 */
	bool assignPackage(unique_ptr<WorkerPackage> pkg);

	/** Set the number of microseconds to sleep between checks
	 *
	 * @param us The number of microseconds
	 */
	void setSleep(std::chrono::microseconds us);
};

}
#endif
