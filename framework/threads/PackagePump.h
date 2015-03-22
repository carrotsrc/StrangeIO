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
#ifndef PACKAGEPUMP_H
#define PACKAGEPUMP_H
#include "common.h"
#include "WorkerPackage.h"

namespace RackoonIO {


class PackagePump {
public:
	PackagePump();

	void addPackage(std::unique_ptr<WorkerPackage>);
	std::unique_ptr<WorkerPackage> nextPackage();

	int getLoad();
private:
	std::vector<std::unique_ptr<WorkerPackage>> mQueue;
	std::mutex mQueueMutex;

};

}
#endif
