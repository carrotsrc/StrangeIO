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
	, m_load(0)
	, m_task_queue(0)
	, m_max_queue(0)
{ }

void loop::add_listener(event_type type, event_callback callback) {
	auto it = m_listeners.find(type);
	if(it == m_listeners.end()) {
		auto p = m_listeners.insert(std::make_pair(type, callback_vec()));
		it = p.first;
	}

	it->second.push_back(callback);
}

void loop::add_event(msg_uptr message) {
	if(listeners(message->type) == 0) {
		return;
	}

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
		 * 2) A list is queue but is not yet being
		 *    processed by a thread
		 *    head: ptr tail: ptr
		 *
		 */
		std::lock_guard<std::mutex> lock(m_tail_mutex);
		
		if(m_tail_ptr == reinterpret_cast<std::uintptr_t>(nullptr)) {
			m_tail_ptr = reinterpret_cast<std::uintptr_t>(item);
			m_head = m_tail = item;
			retask = true;

		} else {
			m_tail->next = item;
			m_tail = item;
			m_tail_ptr = reinterpret_cast<std::uintptr_t>(item);
		}

	}
#if DEVBUILD
	m_load++;
#endif
	if(retask) add_task(std::bind(&loop::cycle_events, this));
}

void loop::cycle_events() {
	// we are starting a new list here
	auto node = m_head;
	while(node != nullptr) {
		m_task_queue++;
		auto sptr = msg_sptr(std::move(node->ptr));

		// Inform all the listeners
		for(auto& listener : m_listeners[sptr->type]) {
			listener(sptr);
		}

		if(m_tail_ptr == reinterpret_cast<std::uintptr_t>(node)) {
			/* We are on the tail so have to deal with
			 * a couple of states
			 *
			 * 1) No new task has been added so
			 *    reset the queue
			 *
			 * 2) Another task has been added after
			 *    we have acquired the lock, so we
			 *    continue as usual
			 */
			std::lock_guard<std::mutex> lock(m_tail_mutex);

			/* Check if it is still the tail after we acquire
			 * the lock
			 */
			if(m_tail_ptr == reinterpret_cast<std::uintptr_t>(node)) {
				/* we're still on the last node
				 * so we have to reset the the
				 * task queue. Once released, the
				 * next locker will start a new
				 * task
				 */
				m_tail_ptr = reinterpret_cast<std::uintptr_t>(nullptr);
				m_tail = nullptr;
				m_head = nullptr;
#if DEVBUILD
				if(m_task_queue > m_max_queue) {
					m_max_queue = m_task_queue;
				}

				m_task_queue = 0;
#endif
			}
			// lock released
		}

		auto next = node->next; //  could be nullptr
		delete node;
#if DEVBUILD
		m_load--;
#endif
		node = next;
	}
}

unsigned int loop::listeners(event_type type) {
	auto it = m_listeners.find(type);
	if(it == m_listeners.end()) return 0;
	return it->second.size();
}

unsigned int loop::size() {
	return m_listeners.size();
}

#if DEVBUILD
unsigned int loop::load() {
	return m_load;
}

unsigned int loop::max_queue() {
	return m_max_queue;
}
#endif
