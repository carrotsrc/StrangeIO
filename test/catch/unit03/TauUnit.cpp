#include "TauUnit.hpp"
using namespace strangeio::component;

#define TauAudioIn 0

TauUnit::TauUnit(std::string label)
: unit(unit_type::dispatch, "Tau", label),
m_init_count(0), m_feed_count(0), m_block_count(0), m_ptr(nullptr) { 
	add_input("audio_in");
}

void TauUnit::set_configuration(std::string key, std::string value) {
	if(key == "test_config") {
		m_config = std::stoi(value);
	}
}

void TauUnit::feed_line(memory::cache_ptr samples, int id) {
	m_block_count = samples.num_blocks();
	m_ptr = samples.get();
}

cycle_state TauUnit::cycle() {
	return cycle_state::complete;
}

cycle_state TauUnit::init() {
	m_init_count++;
		register_metric(profile_metric::period, 512);
	return cycle_state::complete;
}

// Checks
int TauUnit::init_count() const {
	return m_init_count;
}

int TauUnit::feed_count() const {
	return m_feed_count;
}

int TauUnit::block_count() const {
	return m_block_count;
}

int TauUnit::config_check() const {
	return m_config;
}

UnitBuilder(TauUnit);