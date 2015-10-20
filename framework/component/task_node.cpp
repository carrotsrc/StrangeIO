#include <iostream>
#include "framework/component/task_node.hpp"

using namespace strangeio;
using namespace strangeio::component;

task_node::task_node ()
	: thread::task_utility ()
{ 
	add_input("in");
	add_output("out");
}

task_node::~task_node() {
}

void task_node::feed_line(memory::cache_ptr samples, int line) {
	feed_out(samples, 0);
}

cycle_state task_node::cycle_line(cycle_type type) {
	auto out = this->get_output(0);
	add_task([out, type]() {
		std::cout << "Split task" << std::endl;
		out->to->unit->cycle_line(type);
	});
	return cycle_state::complete;
}

void task_node::sync_line(sync_profile& profile, sync_flag flags, unsigned int line) {
	auto out = get_output(0);
	out->to->unit->sync_line(profile, flags, out->to->id);
}