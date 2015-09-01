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
#include "framework/thread/PackagePump.hpp"

using namespace strangeio::Thread;

PackagePump::PackagePump() {
}
PackagePump::~PackagePump() {
	for(auto it = m_queue.begin(); it != m_queue.end();) {
		(*it).reset();
		it = m_queue.erase(it);
	}
}

void PackagePump::add_package(std::unique_ptr<WorkerPackage> pkg) {
	m_queue_mutex.lock();
		m_queue.push_back(std::move(pkg));
	m_queue_mutex.unlock();
}

std::unique_ptr<WorkerPackage> PackagePump::next_package() {
	std::unique_ptr<WorkerPackage> pkg = nullptr;
	m_queue_mutex.lock();
		if(m_queue.size() > 0) {
			auto it = m_queue.begin();
			pkg = std::unique_ptr<WorkerPackage>(std::move(*it));
			m_queue.erase(it);
		}
	m_queue_mutex.unlock();

	return std::move(pkg);
}

int PackagePump::get_load() {
	return m_queue.size();
}
