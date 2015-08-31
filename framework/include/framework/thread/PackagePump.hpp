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
#ifndef PACKAGEPUMP_HPP
#define PACKAGEPUMP_HPP

#include <memory>
#include <vector>
#include <mutex>

#include "framework/fwcommon.hpp"
#include "framework/thread/WorkerPackage.hpp"

namespace StrangeIO {
namespace Thread {

/** A pump for storing and retrieving WorkerPackage objects
 *
 * This is used by the RackQueue as the store and pump for
 * the packages of jobs sent by units to be used by the
 * thread pool.
 *
 * This object is thread-safe but will block on the mutex
 */
class PackagePump {
public:
	PackagePump();
	~PackagePump();

	/** Add a new worker package to the store
	 *
	 * @note This method will block on the mutex
	 *
	 * @param pkg The unique_ptr to a worker package
	 */
	void add_package(std::unique_ptr<WorkerPackage> pkg);

	/** Get the next WorkerPackage from the store
	 *
	 * @note This method will block on the mutex
	 *
	 * @return A unique_ptr to the next WorkerPackage; otherwise nullptr if exhausted
	 */
	std::unique_ptr<WorkerPackage> next_package();

	/** Get the number of packages in the pump */
	int get_load();
private:
	std::vector<std::unique_ptr<WorkerPackage>> m_queue; ///< A vector queue of worker packages
	std::mutex m_queue_mutex; ///< The mutex for accessing the queue

};

} // Thread
} // StrangeIO
#endif
