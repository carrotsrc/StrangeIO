#ifndef ZETA_HPP__
#define ZETA_HPP__
#ifdef __linux__

#include "framework/component/unit.hpp" // Base class: strangeio::component::unit

class Zeta : public strangeio::component::unit
{
public:
	Zeta(std::string label);
	~Zeta();

public:
	strangeio::component::cycle_state cycle();
	void feed_line(strangeio::memory::cache_ptr samples, int line);
	strangeio::component::cycle_state init();
	void set_configuration(std::string key, std::string value);

private:

};


#endif // linux check
#endif