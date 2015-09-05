#ifndef __BASICUNIT_HPP__
#define __BASICUNIT_HPP__

#include "framework/component/unit.hpp" // Base class: StrangeIO::Component::Unit

class BasicUnit : public strangeio::component::unit
{
public:
	BasicUnit(std::string label);
	~BasicUnit();

public:
	strangeio::component::cycle_state cycle();
	void feed_line(strangeio::memory::cache_ptr samples, int line);

	strangeio::component::cycle_state init();
	void set_configuration(std::string key, std::string value);
};

#endif // __BASICUNIT_HPP__
