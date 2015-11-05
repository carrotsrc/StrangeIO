#include "framework/spec/combine.hpp"
using namespace strangeio::component;
using namespace strangeio::spec;
combine::combine(std::string model, std::string label)
	: unit(unit_type::combine, model, label)
{

}

#include <iostream>
void combine::sync_line(sync_profile& profile, sync_flag flags, unsigned int line) {
	if(flags & (sync_flag)sync_flags::upstream) {

		/* combine has special behaviour when upstream
		 * changes
		 */
		if(profile.state == (int)line_state::active) {
			// turning a line on
			// we need to activate the input state
			m_input_state[line] = true;

			if(global_profile().state ==  (int)line_state::inactive) {
				// Our line state is inactive, so we need to flip it
				register_metric(profile_metric::state, (int)line_state::active);

				// state change so propogate the change down the line
				return continue_sync(profile, flags);
			}
		} else {
			
			// turning a line off
			m_input_state[line] = false;

			if(global_profile().state ==  (int)line_state::active) {
				
				// our line state is active
				
				// check if all the other lines are the same
				if(!all_inactive()) return;
				// All input channels are inactive, so change state
				register_metric(profile_metric::state, (int)line_state::inactive);
				/* since no inputs are active now,
				 * we need to propogate the state
				 * change down the line
				 */
				return continue_sync(profile, flags);
			}
		}
	} else {
		/* anything else we just handle by
		 * default
		 */
		unit::sync_line(profile, flags, line);
	}
}

#include <iostream>
void combine::init_input_states() {
	m_input_state.clear();
	for(auto i = 0u; i < inputs().size(); ++i) {
		m_input_state.push_back(false);
	}
}

bool combine::input_active(unsigned int id){
	if(id >= m_input_state.size()
	|| !get_input(id)->connected
	|| !m_input_state[id]) {
		return false;
	}

	return true;
}

bool combine::all_inactive() {
	for(auto act : m_input_state) {
		if(act) return false;
	}

	return true;
}

bool combine::all_active() {
	for(auto act : m_input_state) {
		if(!act) return false;
	}

	return true;
}
