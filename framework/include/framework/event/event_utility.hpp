#ifndef __EVENT_UTILITY_HPP_1441290995__
#define __EVENT_UTILITY_HPP_1441290995__
#include "framework/event/loop_utility.hpp"
#include "framework/event/msg.hpp"
namespace strangeio {
namespace event	{

class event_utility {
public:
	event_utility();
	void set_loop_utility(loop_utility* utility);
protected:
	void add_listener(event_type type, event_callback callback);
	void add_event(msg_uptr message);

private:
	loop_utility* m_loop;
};

} // event
} // strangeio
#endif