#ifndef THETA__HPP__
#define THETA__HPP__

#include "unit.hpp" // Base class: strangeio::component::unit

class Theta : public strangeio::component::unit
{
public:
	Theta(std::string label);
	~Theta();

public:
	cycle_state cycle();
	void feed_line(strangeio::memory::cache_ptr samples, int line);
	cycle_state init();
	void set_configuration(std::string key, std::string value);
};

#endif // THETA__HPP__
