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
#ifndef __WORKERPACKAGE_HPP__
#define __WORKERPACKAGE_HPP__
#include <functional>

namespace strangeio {
namespace thread {


/** An object that encapsulates a task to be passed onto a work thread
 *
 * @note The callback method has no parameters - this is because it is essentially
 * Unit running in parallel. The task has access to the whole
 * state of the respective RackUnit already.
 */
class pkg {
public:
	const std::function<void()> run; ///< The task function to run

	/** instantiate the package with a task method
	 *
	 * @param cbRun The callback method for the task
	 */
	pkg(std::function<void()> cbRun) :
	run(cbRun) { }

	~pkg() {
	}
};

} // Thread
} // StrangeIO
#endif
