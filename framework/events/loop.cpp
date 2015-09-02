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
#include "framework/event/loop.hpp"

using namespace strangeio::event;



loop::loop() {
}

void loop::init(short num_events) {
	while(num_events-- > 0)
		m_listeners.insert(
			std::make_pair(
			num_events,std::vector<event_callback>()
			));
}

void loop::add_listener(event_type type, event_callback callback) {
	m_listeners[type].push_back(callback);
}

void loop::add_event(msg_uptr message) {
	m_queue.push_back(std::move(message));
	this->cv.notify_one();
}

void loop::cycle() {
	m_data = false;
	std::vector< std::unique_ptr<msg> >::iterator qit;
	std::unique_ptr<msg> ptr;
	std::unique_lock<std::mutex> mlock(evLock, std::defer_lock);
	m_active = true;
	m_running = true;
	while(m_running) {
		mlock.lock();
		cv.wait(mlock, [this]{ return this->unblock(); });
			if(!m_running) {
				m_queue.clear();
				mlock.unlock();
				break;
			}

			ptr = nullptr;
			if(m_queue.size()) {
				qit = m_queue.begin();
				ptr = std::move(*qit);
				m_queue.erase(qit);

				if(!m_queue.size())
					m_data = false;
			}
			else {
				m_data = false;
			}
		mlock.unlock();

		if(ptr != nullptr)
			distribute(std::move(ptr));
	}
	m_active = false;
}



void loop::distribute(msg_uptr message) {
	msg_sptr shr(std::move(message));
	event_type type = shr->type;

	for(auto& callback : m_listeners[type]) {
		callback(shr);
	}
}

void loop::start() {
	m_active = false;
	m_running = false;
	m_thread = std::thread(&loop::cycle, this);
	while(!m_running)
		continue;
}

void loop::stop() {
	m_running = false;
	cv.notify_one();
	m_thread.join();
}

bool loop::unblock() {
	if(m_data || !m_running)
		return true;

	return false;
}

bool loop::running() {
	return m_running;
}

bool loop::active() {
	return m_active;
}

void loop::framework_init() {

}
