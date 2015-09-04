#include "Zeta.hpp"
using namespace strangeio;
using namespace strangeio::component;
#include <math.h>

Zeta::Zeta(std::string label)
	: unit(unit_type::dispatch, "Zeta", label)
{
	add_input("audio_in");
}

Zeta::~Zeta() {

}

cycle_state Zeta::cycle() {

	return cycle_state::complete;
}

void Zeta::feed_line(memory::cache_ptr samples, int line) {

}

cycle_state Zeta::init() {
	register_metric(profile_metric::channels, 2);
	log("Initialised");
	return cycle_state::complete;
}

void Zeta::set_configuration(std::string key, std::string value) {

}
