#ifndef CATCH_COMBINE_HPP
#define CATCH_COMBINE_HPP
#include "catch.hpp"
#include "framework/alias.hpp"
#include "framework/spec/combine.hpp"
class Combine : public siospc::combine {
public:
	Combine(std::string label)
	: siospc::combine("Combine", label) {
		add_input("c1");
		add_input("c2");
		init_input_states();
	}

	void feed_line(strangeio::memory::cache_ptr samples, int line) {
		
	}

protected:
	strangeio::component::cycle_state cycle() {
		return siocom::cycle_state::complete;
	};

	strangeio::component::cycle_state init() {
		return siocom::cycle_state::complete;
	};
};

#endif