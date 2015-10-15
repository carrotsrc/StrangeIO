#include "framework/component/utility_container.hpp"
using namespace strangeio;
using namespace strangeio::component;


utility_container::utility_container()
	: m_cache(nullptr)
	, m_queue(nullptr)
	, m_loop(nullptr)
	, m_midi(nullptr)
{ }

utility_container::~utility_container() {
}


void utility_container::set_cache_utility(memory::cache_utility* cache) {
	m_cache = cache;
}

memory::cache_utility* utility_container::get_cache_utility() {
	return m_cache;
}

void utility_container::set_queue_utility(thread::queue_utility* queue) {
	m_queue = queue;
}

thread::queue_utility* utility_container::get_queue_utility() {
	return m_queue;
}

void utility_container::set_loop_utility(event::loop_utility* loop) {
	m_loop = loop;
}

event::loop_utility* utility_container::get_loop_utility() {
	return m_loop;
}

midi::midi_handler* utility_container::get_midi_handler() {
	return m_midi;
}

void utility_container::set_midi_handler(midi::midi_handler* midi) {
	m_midi = midi;
}
