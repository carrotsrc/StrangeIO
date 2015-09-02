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



loop::loop()
	: task_utility()
	, m_head(nullptr)
	, m_tail(nullptr)
	
{
}

void loop::init(short num_events) {

	while(num_events-- > 0) {

		m_listeners.insert(
			std::make_pair(
				num_events, std::vector<event_callback>()
			)
		);

	}
}

void loop::add_listener(event_type type, event_callback callback) {
	m_listeners[type].push_back(callback);
}

void loop::add_event(msg_uptr message) {
	event_list *item = new event_list();
	item->ptr = std::move(message);
	bool retask = false;
	
	{
		/* point of contention is around the tail of the list
		 * because it is around the tail that reconfiguration
		 * occurs. Several states to handle:
		 * 
		 * 1) A completely reset state where no events
		 *    are queued or being processed
		 *    head: nullptr tail: nullptr
		 * 
		 * 2) Worker has finished so nullptrs the tail
		 *    allowing a new list to be queue for work
		 *    head: nullptr, tail: nullptr
		 * 
		 * 3) A list is queue but is not yet being
		 *    processed by a thread
		 *    head: ptr tail: ptr
		 *
		 */
		std::lock_guard<std::mutex> lock(m_tail_mutex);
		
		if(m_tail == nullptr) {
			m_tail = item;
			if(m_head == nullptr) {
				retask = true;
				m_head = item;
			}
		} else {
			m_tail->next = item;
			m_tail = item;
		}

	}

	if(retask) add_task(std::bind(&loop::cycle_events, this));
}

void loop::cycle() {
	/*m_data = false;
	std::vector< std::unique_ptr<msg> >::iterator qit;
	std::unique_ptr<msg> ptr;
	std::unique_lock<std::mutex> mlock(m_tail_mutex, std::defer_lock);
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
	*/
}

void loop::distribute(msg_uptr message) {
	msg_sptr shr(std::move(message));
	event_type type = shr->type;

	for(auto& callback : m_listeners[type]) {
		callback(shr);
	}
}

void loop::framework_init() {

}

void loop::cycle_events() {
	
}