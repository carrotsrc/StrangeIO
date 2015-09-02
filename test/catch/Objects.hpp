#include <memory>
#include "framework/fwcommon.hpp"
#include "framework/memory/cache_ptr.hpp"
#include "framework/component/unit.hpp"
#include "framework/component/rack.hpp"


using namespace strangeio;
using namespace strangeio::component;

class AlphaUnit : public unit {
public:
	AlphaUnit(std::string label)
	: unit(unit_type::mainline, "Alpha", label),
	m_init_count(0), m_feed_count(0)
	{ };

	void feed_line(memory::cache_ptr samples, int id) {
		m_feed_count++;
	}

	void set_configuration(std::string key, std::string value) {
		if(key == "test_key") {
			m_config_test = std::stoi(value);
		}
	}

	void delayed_constructor() {
		add_input("power");
		add_output("audio");
	}

	cycle_state cycle() {
		return cycle_state::complete;
	}

	cycle_state init() {
		m_init_count++;

		register_metric(profile_metric::latency, 1);
		register_metric(profile_metric::fs, 44100);
		register_metric(profile_metric::channels, 2);
		register_metric(profile_metric::period, 1024);
		register_metric(profile_metric::drift, 0.10f);

		return cycle_state::complete;
	}

	// Checks
	int init_count() const {
		return m_init_count;
	}

	int feed_count() const {
		return m_feed_count;
	}

	int config_test() const {
		return m_config_test;
	}

private:
	int m_init_count, m_feed_count, m_config_test;

};

class MuUnit : public unit {
public:
	MuUnit(std::string label)
	: unit(unit_type::mainline, "Mu", label),
	m_init_count(0), m_feed_count(0), m_midi_count(0) { 
		register_midi_handler("mu_bind",[this](midi::msg){
			m_midi_count++;
		});
	};

	void feed_line(memory::cache_ptr samples, int id) {
		m_feed_count++;
	}

	void set_configuration(std::string key, std::string value) {
	}

	cycle_state cycle() {
		return cycle_state::complete;
	}

	cycle_state init() {
		m_init_count++;
		return cycle_state::complete;
	}

	// Checks
	int init_count() const {
		return m_init_count;
	}

	int feed_count() const {
		return m_feed_count;
	}

	int midi_count() const {
		return m_midi_count;
	}

private:
	int m_init_count, m_feed_count, m_midi_count;

};

#define OmegaAudio 0
#define EpsilonAudioIn 0
class OmegaUnit : public unit {
public:
	OmegaUnit(std::string label)
	: unit(unit_type::mainline, "Omega", label),
	m_init_count(0), m_feed_count(0)
	{ };

	void feed_line(memory::cache_ptr samples, int id) {
		m_feed_count++;
	}

	void delayed_constructor() {
		add_input("power");
		add_output("audio");
	}
	
	void set_configuration(std::string, std::string) {}

	cycle_state cycle() {
		feed_out(memory::cache_ptr((PcmSample*)0xFEED, 1, nullptr), OmegaAudio);
		return cycle_state::complete;
	}

	cycle_state init() {
		m_init_count++;

		register_metric(profile_metric::latency, 1);
		register_metric(profile_metric::fs, 44100);
		register_metric(profile_metric::channels, 2);
		register_metric(profile_metric::drift, 0.10f);

		return cycle_state::complete;
	}

	// Checks
	int init_count() const {
		return m_init_count;
	}

	int feed_count() const {
		return m_feed_count;
	}

private:
	int m_init_count, m_feed_count, m_config_test;

};

class EpsilonUnit : public unit {
public:
	EpsilonUnit(std::string label)
	: unit(unit_type::dispatch, "Epsilon", label),
	m_init_count(0), m_feed_count(0), m_feed_check(0.0f)
	{ };

	void feed_line(memory::cache_ptr samples, int id) {
		if(id != EpsilonAudioIn) return;

		if(samples.get() == (PcmSample*)0xFEED) {
			m_feed_check = 0.9f;
		}
		m_feed_count++;
	}

	void set_configuration(std::string key, std::string value) {
	}

	void delayed_constructor() {
		add_input("audio_in");
	}

	cycle_state cycle() {
		return cycle_state::complete;
	}

	cycle_state init() {
		m_init_count++;

		register_metric(profile_metric::latency, 2);
		register_metric(profile_metric::drift, 15);
		register_metric(profile_metric::period, 1024);

		return cycle_state::complete;
	}

	// Checks
	int init_count() {
		return m_init_count;
	}

	int feed_count() {
		return m_feed_count;
	}

	float feed_check() {
		return m_feed_check;
	}

private:
	int m_init_count, m_feed_count;
	float m_feed_check;

};

#define DeltaCA 0
#define DeltaCB 1
#define DeltaAudio 0
class DeltaUnit : public unit {
public:
	DeltaUnit(std::string label)
	: unit(unit_type::combine, "Delta", label),
	m_init_count(0), m_feed_count(0), m_partial_count(0), 
	m_feed_check(0.0f)
	{ 
		add_input("channel_a");
		add_input("channel_b");
		add_output("audio");
	};

	~DeltaUnit() {
	}

	void feed_line(memory::cache_ptr samples, int id) {

		if( id == DeltaCA ) {
			m_ca = samples;
		} else if( id == DeltaCB) {
			m_cb = samples;
		}

		m_feed_count++;
	}

	void set_configuration(std::string key, std::string value) {
	}

	cycle_state cycle() {
		auto state = cycle_state::complete;

		if(input_connected(DeltaCA) && m_ca == false) {
			state = cycle_state::partial;
		} else if(input_connected(DeltaCB) && m_cb == false) {
			state = cycle_state::partial;
		} else {
			feed_out(m_ca, DeltaAudio);
		}

		if(state == cycle_state::partial) {
			m_partial_count++;
		}

		return state;
	}

	cycle_state init() {
		m_init_count++;

		register_metric(profile_metric::latency, 2);
		register_metric(profile_metric::drift, 15);

		return cycle_state::complete;
	}

	// Checks
	int init_count() {
		return m_init_count;
	}

	int feed_count() {
		return m_feed_count;
	}

	float feed_check() {
		return m_feed_check;
	}

	int partial_count() {
		return m_partial_count;
	}

private:
	int m_init_count, m_feed_count, m_partial_count;
	float m_feed_check;

	memory::cache_ptr m_ca, m_cb;

};


#define PhiAudio 0
class PhiUnit : public unit {
public:
	PhiUnit(std::string label)
	: unit(unit_type::mainline, "Phi", label),
	m_init_count(0), m_feed_count(0)
	{ 
		add_output("audio");
	}

	void set_configuration(std::string, std::string) {}

	void feed_line(memory::cache_ptr samples, int id) {
		m_feed_count++;
	}

	cycle_state cycle() {
		auto samples = cache_alloc(5);
		feed_out(samples, PhiAudio);
		return cycle_state::complete;
	}

	cycle_state init() {
		m_init_count++;
		return cycle_state::complete;
	}

	// Checks
	int init_count() const {
		return m_init_count;
	}

	int feed_count() const {
		return m_feed_count;
	}

private:
	int m_init_count, m_feed_count;

};


#define TauAudioIn 0

class TauUnit : public unit {
public:
	TauUnit(std::string label)
	: unit(unit_type::dispatch, "Tau", label),
	m_init_count(0), m_feed_count(0),
	m_block_count(0), m_ptr(nullptr) { 
		add_input("audio_in");
	}

	void feed_line(memory::cache_ptr samples, int id) {
		m_block_count = samples.num_blocks();
		m_ptr = samples.get();
		m_feed_count++;
	}

	cycle_state cycle() {
		return cycle_state::complete;
	}

	void set_configuration(std::string, std::string) {}

	cycle_state init() {
		m_init_count++;
		return cycle_state::complete;
	}

	// Checks
	int init_count() const {
		return m_init_count;
	}

	int feed_count() const {
		return m_feed_count;
	}

	int block_count() const {
		return m_block_count;
	}
	
	const PcmSample* ptr() {
		return m_ptr;
	}

private:
	int m_init_count, m_feed_count, m_block_count;
	const PcmSample* m_ptr;

};



