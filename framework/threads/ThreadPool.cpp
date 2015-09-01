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


using namespace strangeio::Thread;

ThreadPool::ThreadPool() :
m_size(0), m_running(false) 
{ }

ThreadPool::ThreadPool(int num_threads):
m_size(num_threads), m_running(false)
{ }

ThreadPool::~ThreadPool() {

	if(m_running) stop();
	for(auto& th : m_pool) {
			delete th;
	}
}
void ThreadPool::set_size(int num_threads) {
	m_size = num_threads;
}

int ThreadPool::size() {
	return m_size;
}

void ThreadPool::init(std::condition_variable *cv) {
	for(int i = 0; i < m_size; i++) {
		m_pool.push_back(new WorkerThread(cv));
	}
}

void ThreadPool::stop() {
	if(!m_running) return;
	for(auto& th : m_pool) {
		th->stop();
	}
	m_running = false;
}

WorkerThread* ThreadPool::get_thread(int index) {
	return m_pool[index];
}

WorkerThread* ThreadPool::operator[] (int index) {
	return get_thread(index);
}

void ThreadPool::start() {
	for(auto& th : m_pool) {
		th->start();
		auto running = false;

		while(!running)
			running = th->is_running();
	}
	m_running = true;
}
