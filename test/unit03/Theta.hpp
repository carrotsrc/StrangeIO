#ifndef THETA__HPP__
#define THETA__HPP__


#include "framework/spec/mainline.hpp"

class Theta : public strangeio::spec::mainline
{
public:
	Theta(std::string label);
	~Theta();

public:
	strangeio::component::cycle_state cycle();
	void feed_line(strangeio::memory::cache_ptr samples, int line);
	strangeio::component::cycle_state init();
	strangeio::component::cycle_state resync();

private:
	float m_f1, m_2pi, m_x;
};

#endif // THETA__HPP__
