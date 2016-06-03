/* 
 * File:   backend.hpp
 * Author: charlie
 *
 * Created on 15 October 2015, 13:32
 */

#ifndef BACKEND_HPP
#define	BACKEND_HPP
#include "framework/component/component.hpp"
#include "framework/component/mount.hpp"
namespace strangeio {
namespace component {

struct backend_profile {
	profile_duration sync_duration;
	profile_duration cycle_duration;
};

/**
 * This encapsulates the cycling part of the rack
 * 
 * This class provides the functionality for performing cycles on the units that
 * are connected to the rack
 */
class backend 
	: public mount
{
public:
	backend();
	virtual ~backend();
	
	/**
	 * Perform a sync down the daisychains
	 * @param flags Flags to sync with
	 */
	void sync(sync_flag flags);
	
	/**
	 * Perform a sync with a specified profile
	 * @param profile The profile to send down the daisychains
	 * @param flags The flags to sync with
	 */
	void sync(sync_profile& profile, sync_flag flags);
	
	/**
	 * Perform a cycle on the daisychains
	 * 
	 * The cycle can consist of:
	 *  - ac : Process a period of samples
	 *  - sync : Perform a sync
	 *  - warmup : Initialise the units on the daisychains
	 * 
	 * @param type The type of cycle
	 * @return The state at the end of the callstack (complete, partial or error)
	 */
	cycle_state cycle(cycle_type type = cycle_type::ac);

	// Profile stats
	
	/**
	 * Get the profile for the backend
	 * @return The current profile
	 */
	const backend_profile& profile();
	
	/**
	 * Get the global sync profile of the backend
	 * 
	 * @return reference to global sync profile 
	 */
	const sync_profile& global_profile();
	
	/**
	 * perform a line profile sync with a particular mainline
	 * 
	 * @param profile Reference to a profile to send down the line
	 * @param mainline The string name of the mainline
	 * @return true if successful; otherwise error
	 */
	bool profile_line(component::sync_profile& profile, std::string mainline);

protected:
	backend_profile m_rack_profile;
	sync_profile m_global_profile;

	virtual void resync() = 0;
private:
	// Profile methods
	void profile_sync(sync_flag flags);

};

}
}	
#endif	/* BACKEND_HPP */

