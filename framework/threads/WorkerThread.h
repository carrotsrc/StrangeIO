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
#include "WorkerPackage.h"
namespace RackoonIO {

class WorkerThread {
	bool busy;
	bool running;

	std::thread *worker;
	unique_ptr<WorkerPackage> current;
	std::chrono::microseconds uSleep;

	void process();

	std::mutex pkg_lock;
public:
	WorkerThread(bool = false);
	void start();
	void stop();
	bool isBusy();

	bool assignPackage(unique_ptr<WorkerPackage>);
	void setSleep(std::chrono::microseconds);
};

}
#endif
