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

class backend 
	: public mount
{
public:
	backend();
	virtual ~backend();
	
	void sync(sync_flag flags);
	void sync(sync_profile& profile, sync_flag flags);
	cycle_state cycle(cycle_type type = cycle_type::ac);

	// Profile stats
	const backend_profile& profile();
	const sync_profile& global_profile();
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

