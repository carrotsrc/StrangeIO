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
#ifndef PACKAGEQUEUE_HPP
#define PACKAGEQUEUE_HPP

#include "framework/thread/WorkerThread.hpp"
#include "framework/thread/PackagePump.hpp"
#include "framework/thread/ThreadPool.hpp"

namespace strangeio {
namespace Thread {

/** The interface for tasking worker threads
 *
 * This object acts as the handler for the worker threads
 * and is used to queue and distribute tasks to the worker
 * threads via WorkPackage objects
 */
class PackageQueue {
public:
	/** Sets the number of threads in the pool
	 *
	 * @param numThread The number of threads in the pool
	 */
	PackageQueue(int num_thread);

	~PackageQueue();

	/** Set the number of threads in the pool
	 *
	 * @param numThread the number of threads in the pool
	 */
	void set_size(int num_thread);

	/** Get the size of the thread pool
	 *
	 * @return The number of threads
	 */
	int size();

	/** Start the queue and thread pool
	 */
	void start();

	/** Stop all the threads
	 */
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
	void add_package(std::function<void()> run);

	/** Get the current load on the pump
	 */
	int get_load();

	/** Get running state
	 */
	bool is_running();

	/** Get active state
	 */
	bool is_active();

#if DEVBUILD
	WorkerThread* operator[](int index) {
		if(index >= m_pool.size()) return nullptr;
		return m_pool[index];
	};
#endif
private:
	ThreadPool m_pool; ///< The ThreadPool of work threads
	PackagePump m_pump; ///< The package pump
	std::condition_variable m_cycle_condition; ///< Conditonal for unblocking the thread
	std::mutex m_mutex;
	std::thread m_waiter; ///< The thread for distributing packages to threads
	std::atomic<bool> m_running; ///< Toggled when the pool and queue are running
	std::atomic<bool> m_active; ///< Toggle when the waiter thread is active
	int m_size;

	/** Method run in thread to cycle WorkerPackage objects
	 *
	 * This method will block until a worker package is available
	 * or a thread has completed its job and is waiting for another
	 * package
	 */
	void cycle();

	/** Assign a package to a thread.
	 *
	 * This will check if there are any threads waiting for a load
	 * and will pass it into the thread for processing. If there are
	 * not threads then it will return false leaving the raw pointer
	 * to the WorkerPackage untouched.
	 *
	 * @param pkg Raw pointer to the WorkerPackage
	 * @return true if the package was passed to thread; otherwise false
	 */
	bool assign(WorkerPackage *pkg);

};

} // Thread
} // StrangeIO
#endif
