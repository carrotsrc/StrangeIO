#include "framework/component/Rack.hpp"
using namespace StrangeIO::Component;
Rack::Rack()
{ }

Rack::~Rack() {

}

void Rack::register_metric(ProfileMetric type, int value) {

	m_global_metrics.insert(
			std::pair<ProfileMetric, int>(type, value)
	);

}
