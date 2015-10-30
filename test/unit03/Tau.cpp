#include "Tau.hpp"
using namespace strangeio::component;

#define TauAudioIn 0

Tau::Tau(std::string label)
: unit(unit_type::dispatch, "Tau", label)
, m_init_count(0)
, m_feed_count(0)
, m_block_count(0)
, m_config_test(0)
, m_ptr(nullptr) 
{ 
	add_input("audio_in");
}

void Tau::set_configuration(std::string key, std::string value) {
	if(key == "test_config") {
		m_config_test = std::stoi(value);
	}
}

std::string Tau::get_configuration(std::string key) {
	if(key == "test_config")
		return std::to_string(m_config_test);

	return std::string();
}

void Tau::feed_line(memory::cache_ptr samples, int id) {
	m_block_count = samples.num_blocks();
	m_ptr = samples.get();
}

cycle_state Tau::cycle() {
	return cycle_state::complete;
}

cycle_state Tau::init() {
	m_init_count++;
		register_metric(profile_metric::period, 512);
	return cycle_state::complete;
}

// Checks
int Tau::init_count() const {
	return m_init_count;
}

int Tau::feed_count() const {
	return m_feed_count;
}

int Tau::block_count() const {
	return m_block_count;
}

int Tau::config_test() const {
	return m_config_test;
}

UnitBuilder(Tau);