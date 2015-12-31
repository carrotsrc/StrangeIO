#ifndef __EVENT_LIST_HPP_1441220646__
#define __EVENT_LIST_HPP_1441220646__
#include <memory>
#include "framework/event/msg.hpp"

namespace strangeio {
namespace event {

	struct event_list {
		msg_uptr ptr;
		event_list* next;
	};
}
}
#endif