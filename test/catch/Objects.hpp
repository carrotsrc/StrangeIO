#include <memory>
#include "framework/fwcommon.hpp"
#include "framework/memory/CachePtr.hpp"
#include "framework/component/Unit.hpp"
#include "framework/component/Rack.hpp"


using namespace StrangeIO;
using namespace StrangeIO::Component;
class AlphaUnit : public Unit {
public:
	AlphaUnit(std::string label)
	: Unit(UnitType::Mainliner, "Alpha", label),
	m_init_count(0), m_feed_count(0)
	{ };

	void feed_line(Memory::CachePtr samples, int id) {
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

	CycleState cycle() {
		return CycleState::Complete;
	}

	CycleState init() {
		m_init_count++;

		register_metric(ProfileMetric::Latency, 1);
		register_metric(ProfileMetric::Fs, 44100);
		register_metric(ProfileMetric::Channels, 2);
		register_metric(ProfileMetric::Period, 1024);
		register_metric(ProfileMetric::Drift, 0.10f);

		return CycleState::Complete;
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

#define OmegaAudio 0
#define EpsilonAudioIn 0
class OmegaUnit : public Unit {
public:
	OmegaUnit(std::string label)
	: Unit(UnitType::Mainliner, "Omega", label),
	m_init_count(0), m_feed_count(0)
	{ };

	void feed_line(Memory::CachePtr samples, int id) {
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

	CycleState cycle() {
		feed_out(Memory::CachePtr((PcmSample*)0xFEED, 1, nullptr), OmegaAudio);
		return CycleState::Complete;
	}

	CycleState init() {
		m_init_count++;

		register_metric(ProfileMetric::Latency, 1);
		register_metric(ProfileMetric::Fs, 44100);
		register_metric(ProfileMetric::Channels, 2);
		register_metric(ProfileMetric::Period, 1024);
		register_metric(ProfileMetric::Drift, 0.10f);

		return CycleState::Complete;
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

class EpsilonUnit : public Unit {
public:
	EpsilonUnit(std::string label)
	: Unit(UnitType::Dispatcher, "Epsilon", label),
	m_init_count(0), m_feed_count(0), m_feed_check(0.0f)
	{ };

	void feed_line(Memory::CachePtr samples, int id) {
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

	CycleState cycle() {
		return CycleState::Complete;
	}

	CycleState init() {
		m_init_count++;

		register_metric(ProfileMetric::Latency, 2);
		register_metric(ProfileMetric::Drift, 15);

		return CycleState::Complete;
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
class DeltaUnit : public Unit {
public:
	DeltaUnit(std::string label)
	: Unit(UnitType::Combiner, "Delta", label),
	m_init_count(0), m_feed_count(0), m_partial_count(0), 
	m_feed_check(0.0f),
	m_ca{Memory::CachePtr()}, m_cb{Memory::CachePtr()}
	{ 
		add_input("channel_a");
		add_input("channel_b");
		add_output("audio");
	};

	~DeltaUnit() {
	}

	void feed_line(Memory::CachePtr samples, int id) {

		if( id == DeltaCA ) {
			//m_ca.swap(samples);
			m_ca = samples;
		} else if( id == DeltaCB) {
			m_cb.swap(samples);
		}

		m_feed_count++;
	}

	void set_configuration(std::string key, std::string value) {
	}

	CycleState cycle() {
		auto state = CycleState::Complete;

		if(input_connected(DeltaCA) && !m_ca) {
			state = CycleState::Partial;
		} else if(input_connected(DeltaCB) && !m_cb) {
			state = CycleState::Partial;
		} else {
			feed_out(m_ca, DeltaAudio);
		}

		if(state == CycleState::Partial) {
			m_partial_count++;
		}

		return state;
	}

	CycleState init() {
		m_init_count++;

		register_metric(ProfileMetric::Latency, 2);
		register_metric(ProfileMetric::Drift, 15);

		return CycleState::Complete;
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

	Memory::CachePtr m_ca, m_cb;

};


