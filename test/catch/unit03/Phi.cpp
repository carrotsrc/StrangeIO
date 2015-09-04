#include "Phi.hpp"
using namespace strangeio::component;

#define PhiAudio 0

Phi::Phi(std::string label)
: unit(unit_type::mainline, "Phi", label),
m_init_count(0), m_feed_count(0) { 
	add_output("audio");
}

void Phi::set_configuration(std::string, std::string) {

}

void Phi::feed_line(memory::cache_ptr samples, int id) {
	m_feed_count++;
}

cycle_state Phi::cycle() {
	auto samples = cache_alloc(5);
	feed_out(samples, PhiAudio);
	return cycle_state::complete;
}

cycle_state Phi::init() {
	m_init_count++;
	register_metric(profile_metric::channels, 2);
	register_metric(profile_metric::fs, 44100);
	return cycle_state::complete;
}

// Checks
int Phi::init_count() const {
	return m_init_count;
}

int Phi::feed_count() const {
	return m_feed_count;
}

UnitBuilder(Phi);