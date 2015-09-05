#ifndef __RACKUTILITYINTERFACE_HPP_1440594958__
#define __RACKUTILITYINTERFACE_HPP_1440594958__
namespace strangeio {
namespace component {

class rack_utility {
public:
	virtual void trigger_sync(sync_flag flags) = 0;
	virtual void trigger_cycle() = 0;
};

} // Component
} // StrangeIO
 
#endif


