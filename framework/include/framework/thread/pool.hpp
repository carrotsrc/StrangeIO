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
#ifndef THREADPOOL_HPP
#define THREADPOOL_HPP
#include <vector>

#include "framework/thread/worker.hpp"


namespace strangeio {
namespace thread {

/** The thread pool
 *
 * This class acts as the interface and handler for the
 * pool of threads it is running inside it.
 *
 * This is a template class for pooling a specific task thread
 * so if the thread changes in the future it will be easy to
 * drop in a different template of thread pool.
 *
 * It is fairly simple; it doesn't dynamically create and
 * destroy threads based on work load but it does the job.
 */
class pool {
	int m_size; ///< The number of threads in the pool
	std::vector< worker* > m_pool; ///< The vector of threads
	bool m_running;

public:
	/** Instantiates a blank pool to be sized later */
	pool();


	/** Sets the number of threads in the pool
	 *
	 * @param nThreads The number of threads
	 */
	pool(int nThreads);

	~pool();

	/** set the number of threads in the pool
	 *
	 * This is used after the object is instantiated
	 *
	 * @param nThreads the number of threads in the pool
	 */
	void set_size(int nThreads);

	/** Get the size of the thread pool
	 *
	 * @return The number of threads in the pool
	 */
	int size();

	/** Start the threads in the pool
	 *
	 * @param cv The condition variable to notify the thread handler
	 */
	void init(std::condition_variable *cv);

	/** Stop the threads */
	void stop();
	
	/** Start the threads */
	void start();

	/** Get the thread with specified index
	 *
	 * @param index The index of the thread
	 * @return A pointer to the thread
	 */
	worker* get_thread(int index);

	/** Array operator for accessing a thread at specified index
	 *
	 * @return A pointer to the thread
	 */
	worker* operator[] (int index);
};


} // Thread
} // StrangeIO
#endif
