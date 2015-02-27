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
	ThreadPool<WorkerThread> *pool; ///< The ThreadPool of work threads
	std::vector< std::unique_ptr<WorkerPackage> > queue; ///< A vector of queued tasks
	std::mutex qmutex; ///< The lock on the queue
	bool running; ///< Toggled when the pool and queue are running

	/** inlined method for loading the threads with a task
	 *
	 * @param it An iterator for the queue of tasks
	 */
	inline void loadThreads(std::vector< std::unique_ptr<WorkerPackage> >::iterator it);
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

	/** Set the number of useconds to sleep for between checks
	 *
	 * @param us The number of microseconds to sleep for
	 */
	void setSleep(std::chrono::microseconds us);

	/** Initialise the queue and thread pool
	 */
	void init();

	/** Start the queue and thread pool
	 */
	void start();

	/** Cycle any tasks and distribute to threads
	 */
	bool cycle();

	/** Get the number of tasks in the queue
	 * 
	 * @return The number of tasks queued
	 */
	int getQueueSize() {
		return queue.size();
	}

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

	/** Add a worker package to the queue for tasking to a thread (nonblocking)
	 *
	 * When an object has a task to run in parallel, it places it
	 * in a worker package, which is put in the queue via this method.
	 *
	 * This method is nonblock, so will directly return if the queue
	 * is locked
	 *
	 * @param run The worker package to run
	 * @return true if the package was placed in the queue; false if the queue was locked
	 */
	bool tryAddPackage(std::function<void()> run);
};

}
#endif
