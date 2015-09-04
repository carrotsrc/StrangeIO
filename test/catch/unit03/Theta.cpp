#include "Theta.hpp"
using namespace strangeio;
using namespace strangeio::component;
#include <math.h>

Theta::Theta(std::string label)
	: unit(unit_type::mainline, "Theta", label)
	, m_f1(432)
	, m_x(0)
{
	add_output("audio");
	m_2pi = M_PI*2;
}

Theta::~Theta() {

}

cycle_state Theta::cycle() {

	auto cptr = cache_alloc(1);
	auto& profile = global_profile();

	for(int  i = 0; i < profile.period; ++i) {
		cptr[i] = sin((m_2pi * m_f1*m_x) / profile.fs);
		cptr[++i] = sin((m_2pi * m_f1*m_x++) / profile.fs);
	}

	feed_out(cptr, 0);
	return cycle_state::complete;
}

void Theta::feed_line(memory::cache_ptr samples, int line) {

}

cycle_state Theta::init() {
	register_metric(profile_metric::channels, 2);
	log("Initialised");
	return cycle_state::complete;
}

void Theta::set_configuration(std::string key, std::string value) {

}

UnitBuilder(Theta);