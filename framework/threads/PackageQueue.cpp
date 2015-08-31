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
#include "framework/thread/PackageQueue.hpp"
using namespace StrangeIO::Thread;

PackageQueue::PackageQueue(int size) {
	m_pool = ThreadPool(size);
	m_size = size;
	m_active = false;
	m_running = false;
}

PackageQueue::~PackageQueue() {
	if(m_running && m_active) {
		m_running = false;
		m_cycle_condition.notify_all();
		m_waiter.join();
		m_active = false;
	}
}

void PackageQueue::set_size(int size) {
	m_pool.set_size(size);
	m_size = size;
}

int PackageQueue::size() {
	return m_pool.size();
}

void PackageQueue::start() {
	m_active = false;
	m_pool.init(&m_cycle_condition);
	m_waiter = std::thread(&PackageQueue::cycle, this);
	m_running = true;
}

void PackageQueue::add_package(std::function<void()> run) {
	// Pump is thread safe
	m_pump.add_package(std::unique_ptr<WorkerPackage>(new WorkerPackage(run)));
	m_cycle_condition.notify_one();
}

void PackageQueue::stop() {
	m_pool.stop();
	m_running = false;
	m_cycle_condition.notify_all();
	m_waiter.join();
	m_active = false;
}

int PackageQueue::get_load() {
	return m_pump.get_load();
}

/*
 * There are two conditions for this trigger a
 * dispatch attempt of another job - a new job
 * has been added or a thread has finished a job.
 *
 * Dispatch should be done centrally within this
 * method to avoid race conditions
 */
void PackageQueue::cycle() {
	m_active = true;
	std::unique_lock<std::mutex> lock(m_mutex);
	WorkerPackage *raw_pkg = nullptr;
	while(m_running) {
		std::unique_ptr<WorkerPackage> pkg = nullptr;

		m_cycle_condition.wait(lock);

		if(!m_running) {
			lock.unlock();
			break;
		}

		if(raw_pkg == nullptr) {
			pkg = m_pump.next_package();
			raw_pkg = pkg.release();
		}

		if(raw_pkg == nullptr) {
			continue;
		}

		if(assign(raw_pkg))
			raw_pkg = nullptr;
	}
}

bool PackageQueue::assign(WorkerPackage *pkg) {
	for(int i = 0; i < m_size; i++) {
		if(m_pool[i]->is_waiting()) {
			m_pool[i]->assign_package(std::unique_ptr<WorkerPackage>(pkg));
			m_pool[i]->notify();
			return true;
		}
	}
	return false;
}

bool PackageQueue::is_running() {
	return m_running;
}

bool PackageQueue::is_active() {
	return m_active;
}
