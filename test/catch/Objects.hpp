#include <memory>
#include "framework/fwcommon.hpp"
#include "framework/component/Unit.hpp"
#include "framework/component/Rack.hpp"

using namespace StrangeIO::Component;

#define OmegaAudio 0
#define EpsilonAudioIn 0
class OmegaUnit : public Unit {
public:
	OmegaUnit(std::string label)
	: Unit(UnitType::Mainliner, "Omega", label),
	m_init_count(0), m_feed_count(0)
	{ };

	void feed_line(PcmSample* samples, int id) {
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
		auto samples = new PcmSample(3);
		samples[0] = 0.3f;  samples[1] = 0.6f; samples[2] = 0.9f;  
		feed_out(samples, OmegaAudio);
		return CycleState::Complete;
	}

	CycleState init() {
		m_init_count++;

		register_metric(ProfileMetric::Latency, 1);
		register_metric(ProfileMetric::Fs, 44100);
		register_metric(ProfileMetric::Channels, 2);
		register_metric(ProfileMetric::Period, 1024);
		register_metric(ProfileMetric::Drift, 10);

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

	void feed_line(PcmSample* samples, int id) {
		if(id != EpsilonAudioIn) return;

		m_feed_check = samples[2];
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
