#ifndef __LOOP_UTILITY_HPP_1441290995__
#define __LOOP_UTILITY_HPP_1441290995__
#include "framework/event/msg.hpp"

namespace strangeio {
namespace event	{

class loop_utility {
public:
	virtual void add_listener(event_type type, event_callback callback) = 0;
	virtual void add_event(msg_uptr message) = 0;
};

} // event
} // strangeio
#endif