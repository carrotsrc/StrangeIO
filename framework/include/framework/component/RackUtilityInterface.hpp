#ifndef __RACKUTILITYINTERFACE_HPP_1440594958__
#define __RACKUTILITYINTERFACE_HPP_1440594958__
namespace StrangeIO {
namespace component {

class RackUtilityInterface {
public:
	virtual void trigger_sync() = 0;
	virtual void trigger_cycle() = 0;
};

} // Component
} // StrangeIO
 
#endif


