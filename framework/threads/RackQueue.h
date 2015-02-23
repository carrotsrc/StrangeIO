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

class RackQueue {
	ThreadPool<WorkerThread> *pool;
	std::vector< std::unique_ptr<WorkerPackage> > queue;
	std::mutex qmutex;
	bool running;

	inline void loadThreads(std::vector< std::unique_ptr<WorkerPackage> >::iterator it);
public:
	RackQueue(int);

	void setSize(int);
	int getSize();

	void setSleep(std::chrono::microseconds);
	void init();
	void start();
	bool cycle();

	int getQueueSize() {
		return queue.size();
	}

	void addPackage(std::function<void()> run);
	bool tryAddPackage(std::function<void()> run);
};

}
#endif
