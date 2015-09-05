#ifndef THETA__HPP__
#define THETA__HPP__

#include "framework/component/unit.hpp" // Base class: strangeio::component::unit

class Theta : public strangeio::component::unit
{
public:
	Theta(std::string label);
	~Theta();

public:
	strangeio::component::cycle_state cycle();
	void feed_line(strangeio::memory::cache_ptr samples, int line);
	strangeio::component::cycle_state init();
	void resync();

private:
	float m_f1, m_2pi, m_x;
};

#endif // THETA__HPP__
