#ifndef UTILITY_CONTAINER_HPP
#define	UTILITY_CONTAINER_HPP

#include "framework/midi/midi_handler.hpp"
#include "framework/memory/cache_utility.hpp"
#include "framework/thread/queue_utility.hpp"
#include "framework/event/loop_utility.hpp"
#include "framework/component/registry_utility.hpp"
#include "registry.hpp"
namespace strangeio {
namespace component {
	

class utility_container {
public:
	utility_container();
	virtual ~utility_container();

	// Utilities
	void set_cache_utility(memory::cache_utility* cache);
	memory::cache_utility* get_cache_utility();
	
	void set_queue_utility(thread::queue_utility* queue);
	thread::queue_utility* get_queue_utility();
	
	void set_loop_utility(event::loop_utility* loop);
	event::loop_utility* get_loop_utility();

	void set_midi_handler(midi::midi_handler* midi);
	midi::midi_handler* get_midi_handler();
	
	void set_registry_utility(component::registry* registry);
	component::registry* get_registry_utility();

protected:
	memory::cache_utility* m_cache;
	thread::queue_utility* m_queue;
	event::loop_utility* m_loop;
	midi::midi_handler* m_midi;
	component::registry* m_registry;
	

private:

};

}
}
#endif	/* UTILITY_CONTAINER_HPP */

