#ifndef __RACKUTILITYINTERFACE_HPP_1440594958__
#define __RACKUTILITYINTERFACE_HPP_1440594958__
namespace strangeio {
namespace component {

class rack_utility {
public:
	virtual void trigger_sync() = 0;
	virtual void trigger_cycle() = 0;
};

} // Component
} // StrangeIO
 
#endif


