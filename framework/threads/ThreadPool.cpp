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
#include "framework/thread/ThreadPool.hpp"


using namespace StrangeIO::Thread;

ThreadPool::ThreadPool() {

}

ThreadPool::ThreadPool(int num_threads) {
	m_size = num_threads;
}

ThreadPool::~ThreadPool() {
	stop();
}
void ThreadPool::set_size(int num_threads) {
	m_size = num_threads;
}

int ThreadPool::get_size() {
	return m_size;
}

void ThreadPool::init(std::condition_variable *cv) {
	
	for(int i = 0; i < m_size; i++) {
		m_pool.push_back(WorkerThread(cv));
		auto running = false;

		while(!running)
			running = m_pool[i].is_running();
	}
}

void ThreadPool::stop() {
	for(auto& th : m_pool) {
		if(th.is_active()) {
			th.stop();
		}
	}
}

WorkerThread& ThreadPool::get_thread(int index) {
	return m_pool[index];
}

WorkerThread& ThreadPool::operator[] (int index) {
	return get_thread(index);
}

