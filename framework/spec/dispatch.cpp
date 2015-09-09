#include "framework/alias.hpp"
#include "framework/spec/dispatch.hpp"

using namespace strangeio::component;
using namespace strangeio::spec;
dispatch::dispatch(std::string model, std::string label)
	: unit(unit_type::dispatch, model, label)
{

}
#include <iostream>
#include <sstream>
void dispatch::sync_line(sync_profile& profile, sync_flag flags, unsigned int line) {
	if((flags & (sync_flag)sync_flags::upstream)) {
		/* Dispatch units have specific behaviour when
		 * handling upstream syncs. The dispatch is the
		 * part of the daisychain that registers a global
		 * state change -- currently the line activating
		 * or deactivating -- and triggers a global sync
		 * 
		 * In other words, the line state of the rack
		 * is determined by the sync profile of the 
		 * dispatch unit on the daisychain.
		 */
		 

		
		 if(profile.state != global_profile().state) {
			/* the state of the rack has changed, so we
			 * need to trigger a global resync.
			 * 
			 * This global resync should be performed within
			 * the current latency window since upstreams syncs
			 * are cycled before global syncs
			 */
			register_metric(profile_metric::state, (int)profile.state);
			std::stringstream ss;

			trigger_sync((sync_flag)sync_flags::glob_sync);
			
			if(profile.state == (int)line_state::active) {
				trigger_cycle();
			}
		 }
	} else {
		/* currently the dispatch unit decides the active
		 * state of the entire rack. This means an active
		 * line sync reaches the dispatch unit, the rack
		 * is set to active in the next global update. If
		 * the line sync has an inactive state, the rack
		 * is set to being inactive in the next global
		 * sync.
		 */
		profile.state = unit_profile().state;
		unit::sync_line(profile, flags, line);
	}
}