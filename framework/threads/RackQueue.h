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
#ifndef RACKQUEUE_H
#define RACKQUEUE_H
#include "WorkerThread.h"
#include "ThreadPool.h"

namespace RackoonIO {

/** The interface for tasking worker threads
 *
 * This object acts as the handler for the worker threads
 * and is used to queue and distribute tasks to the worker
 * threads via WorkPackage objects
 */
class RackQueue {
	ThreadPool pool; ///< The ThreadPool of work threads
	PackagePump mPump;
	std::condition_variable mCycleCondition;
	std::mutex mMutex;
	std::thread mWaiter;
	bool mRunning; ///< Toggled when the pool and queue are running
	int mPoolSize;

	void cycle();
	bool assign(WorkerPackage *pkg);
public:
	/** Sets the number of threads in the pool
	 *
	 * @param numThread The number of threads in the pool
	 */
	RackQueue(int numThread);

	/** Set the number of threads in the pool
	 * 
	 * @param numThread the number of threads in the pool
	 */
	void setSize(int numThread);

	/** Get the size of the thread pool
	 *
	 * @return The number of threads
	 */
	int getSize();

	/** Initialise the queue and thread pool
	 */
	void init();

	/** Start the queue and thread pool
	 */
	void start();

	void stop();

	/** Add a worker package to the queue for tasking to a thread (blocking)
	 *
	 * When an object has a task to run in parallel, it places it
	 * in a worker package, which is put in the queue via this method.
	 *
	 * This method will block until the queue is unlocked
	 *
	 * @param run The worker package to run
	 */
	void addPackage(std::function<void()> run);

	int getPumpLoad();
};

}
#endif
