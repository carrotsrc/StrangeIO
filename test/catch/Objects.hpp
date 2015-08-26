#include <memory>
#include "framework/fwcommon.hpp"
#include "framework/component/Unit.hpp"
#include "framework/component/Rack.hpp"

using namespace StrangeIO::Component;

class OmegaUnit : public Unit {
public:
	OmegaUnit(std::string label)
	: Unit(UnitType::Mainliner, "Omega", label),
	m_init_count(0), m_feed_count(0)
	{ };

	void feed_line(PcmSample* samples, int id) {
		m_feed_count++;
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
		register_metric(ProfileMetric::Drift, 10);

		return CycleState::Complete;
	}

	int init_count() {
		return m_init_count;
	}

private:
	int m_init_count, m_feed_count;

};
