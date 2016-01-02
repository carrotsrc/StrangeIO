#include "framework/alias.hpp"
#include "framework/spec/mainline.hpp"

using namespace strangeio::component;
using namespace strangeio::spec;
mainline::mainline(std::string model, std::string label)
	: unit(unit_type::mainline, model, label)
{

}
#include <iostream>
#include <sstream>
void mainline::sync_line(sync_profile& profile, sync_flag flags, unsigned int line) {
	if((flags & (sync_flag)sync_flags::upstream)) {
		/* A mainline unit determines the upstream state of the line.
		 * This means the mainline will propogate it's unit state
		 * down the line, causing state switches at relevant places
		 */
		if(m_upstream) {
			/* this unit requested an upstream sync
			 * it might need to do some resyncing
			 */
			m_upstream = false;

			resync(flags);
		}

		profile.state = unit_profile().state;
		return continue_sync(profile, flags);

	} else {
		unit::sync_line(profile, flags, line);
	}
}