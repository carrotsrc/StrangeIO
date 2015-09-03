#include "framework/event/event_utility.hpp"

using namespace strangeio::event;

void event_utility::set_loop_utility(loop_utility* utility) {
	m_loop = utility;
}

void event_utility::add_listener(event_type type, event_callback callback) {
	m_loop->add_listener(type, callback);
}

void event_utility::add_event(msg_uptr message) {
	m_loop->add_event(std::move(message));
}
