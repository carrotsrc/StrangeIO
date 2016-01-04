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
#include "framework/thread/pump.hpp"

using namespace strangeio::thread;

pump::pump()
	: m_load(0)
{ }
pump::~pump() {
	for(auto it = m_queue.begin(); it != m_queue.end();) {
		(*it).reset();
		it = m_queue.erase(it);
	}
}

void pump::add_package(std::unique_ptr<pkg> task) {
	m_queue_mutex.lock();
		m_queue.push_back(std::move(task));
		m_load++;
	m_queue_mutex.unlock();
}

std::unique_ptr<pkg> pump::next_package() {
	std::unique_ptr<pkg> task = nullptr;
	m_queue_mutex.lock();
		if(m_queue.size() > 0) {
			auto it = m_queue.begin();
			task = std::unique_ptr<pkg>(std::move(*it));
			m_queue.erase(it);
			m_load--;
		}
	m_queue_mutex.unlock();

	return std::move(task);
}

int pump::get_load() {
	return m_load;
}
