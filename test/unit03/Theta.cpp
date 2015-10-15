#include "Theta.hpp"

#include "framework/alias.hpp"

using namespace strangeio;
using namespace strangeio::component;
#include <math.h>
#if !DEVBUILD
	#error The testing suite requires DEVBUILD to be enabled
#endif

Theta::Theta(std::string label)
	: strangeio::spec::mainline("Theta", label)
	, m_f1(200)
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
	auto total = profile.period*2;
	for(int  i = 0; i < total; i++) {
		auto mval = sin(m_2pi * m_f1 * m_x++ / profile.fs)/2;
		cptr[i] = mval;
		i++;
		cptr[i] = mval;
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
strangeio::component::cycle_state Theta::resync() {
	return strangeio::component::cycle_state::complete;
}

UnitBuilder(Theta);