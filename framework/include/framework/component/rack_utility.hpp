#ifndef __RACKUTILITYINTERFACE_HPP_1440594958__
#define __RACKUTILITYINTERFACE_HPP_1440594958__
namespace strangeio {
namespace component {

/**
 * abstract class for objects to interface with the rack
 */
class rack_utility {
public:
	virtual ~rack_utility() {}
	/**
	 * Trigger a sync on the rack
	 * 
	 * @param flags Flags to sync with
	 */
	virtual void trigger_sync(sync_flag flags) = 0;
	
	/**
	 * Trigger a cycle on the rack
	 */
	virtual void trigger_cycle() = 0;
};

} // Component
} // StrangeIO
 
#endif


