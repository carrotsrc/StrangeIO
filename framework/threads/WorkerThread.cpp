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
#include "framework/thread/WorkerThread.hpp"

using namespace StrangeIO::Thread;

WorkerThread::WorkerThread(std::condition_variable *cv) {
	m_ready_condition = cv;
	m_running = false;
	m_active = false;
	start();
}

void WorkerThread::start() {
	m_running = false;
	m_worker = std::thread(&WorkerThread::process, this);
}

void WorkerThread::stop() {
	m_running = false;
	m_condition.notify_one();
	m_worker.join();
	m_active = false;
}

void WorkerThread::process() {
	std::unique_lock<std::mutex> lock(m_mutex);
	m_loaded = false;
	m_running = m_active = true;
	while(m_running) {
		m_condition.wait(lock);
			if(!m_running) {
				m_current.reset();
				lock.unlock();
				break;
			}
			m_current->run();
			m_loaded = false;
			m_ready_condition->notify_all();
	}
}

bool WorkerThread::is_running() {
	return m_running;
}

bool WorkerThread::is_active() {
	return m_active;
}

bool WorkerThread::assign_package(std::unique_ptr<WorkerPackage> pkg, bool unlock) {
	m_current = std::move(pkg);
	m_loaded = true;
	if(unlock)
		m_mutex.unlock();
	return true;
}

bool WorkerThread::is_loaded() {
	return m_loaded;
}

void WorkerThread::notify() {
	m_condition.notify_all();
}

bool WorkerThread::is_waiting() {
	if(!m_mutex.try_lock()) {
		return false;
	}

	if(m_loaded) {
		m_mutex.unlock();
		return false;
	}
	return true;
}
