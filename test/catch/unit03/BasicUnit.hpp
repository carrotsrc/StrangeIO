#ifndef __BASICUNIT_HPP__
#define __BASICUNIT_HPP__

#include "Unit.hpp" // Base class: StrangeIO::Component::Unit

class BasicUnit : public StrangeIO::Component::Unit
{
public:
	BasicUnit(std::string label);
	~BasicUnit();

public:
	CycleState cycle();
	void feed_line(Memory::CachePtr samples, int line);

	CycleState init();
	void set_configuration(std::string key, std::string value);
};

#endif // __BASICUNIT_HPP__
