#include <chrono>
#include <iostream>
#include "framework/component/rack.hpp"

using namespace strangeio;
using namespace strangeio::component;

using pclock = std::chrono::steady_clock;

rack::rack()
	: m_active(false)
	, m_running(false)
	, m_cache(nullptr)
	, m_queue(nullptr)
	, m_loop(nullptr)
	, m_midi(nullptr)
	, m_global_profile({0})
	, m_resync(false)

{

	m_rack_profile = {
		.sync_duration = profile_duration::zero(),
		.cycle_duration = profile_duration::zero(),
	};

}

rack::~rack() {
	if(m_active) {
		stop();
	}
}

void rack::set_cache_utility(memory::cache_utility* cache) {
	m_cache = cache;
}

memory::cache_utility* rack::get_cache_utility() {
	return m_cache;
}

void rack::set_queue_utility(thread::queue_utility* queue) {
	m_queue = queue;
}

thread::queue_utility* rack::get_queue_utility() {
	return m_queue;
}

void rack::set_loop_utility(event::loop_utility* loop) {
	m_loop = loop;
}

event::loop_utility* rack::get_loop_utility() {
	return m_loop;
}


void rack::add_unit(unit_uptr unit) {

	unit->set_rack(this);

	unit->set_cache_utility(m_cache);
	unit->set_loop_utility(m_loop);
	unit->set_queue_utility(m_queue);

	auto label = unit->ulabel();
	m_mounted.insert(
		std::pair<std::string, unit_sptr>(
			label, unit_sptr(std::move(unit))
		)
	);
}

const std::map<std::string, unit_sptr> & rack::get_units() {
	return m_mounted;
}

unit_wptr rack::get_unit(std::string label) {

	auto u = m_mounted.find(label);

	if(u == m_mounted.end()) {
		return unit_wptr();
	}

	return u->second;
}

bool rack::has_unit(std::string label) {
	auto u = m_mounted.find(label);
	if(u == m_mounted.end()) return false;

	return true;
}

void rack::clear_units() {
	m_mounted.clear();
}

void rack::add_mainline(std::string name) {
	if(m_mainlines.find(name) != m_mainlines.end())
		return;

	m_mainlines.insert(
			std::pair<std::string, unit_wptr>(
				name, unit_wptr()
			)
		);
}

bool rack::connect_mainline(std::string mainline, std::string unit) {

	auto line = m_mainlines.find(mainline);

	if(line == m_mainlines.end()) return false;

	auto wptr = get_unit(unit);
	if(wptr.expired()) return false;

	line->second = wptr;

#if DEVBUILD
	std::cout << "[Connection]\t" << 
		"rack." << mainline << 
		" --> " <<
		unit << ".power" << std::endl;
#endif

	return true;
}

bool rack::connect_units(std::string from, std::string out, std::string to, std::string in) {
	auto ufrom = get_unit(from);
	if(ufrom.expired()) return false;


	auto uto = get_unit(to);
	if(uto.expired()) return false;

	auto from_shr = ufrom.lock();
	auto to_shr = uto.lock();

	auto output_id = from_shr->has_output(out);
	auto input_id = to_shr->has_input(in);

	if(output_id < 0 || input_id < 0) return false;


	auto link_in = const_cast<LinkIn*>(to_shr->get_input(input_id));
	auto ret = from_shr->connect(output_id, link_in);
#if DEVBUILD
	if(ret) {
		std::cout << "[Connection]\t" << 
			from << "." << out << 
			" --> " <<
			to << "." << in << std::endl;
	} else {
		std::cout << "[Connection failure]" << std::endl;
	}
#endif
	return ret;
}

void rack::trigger_sync(sync_flag flags) {
	// don't unflag previous flags
	if(flags) m_resync_flags |= flags;

	m_resync = true;
}

void rack::trigger_cycle() {
	m_trigger.notify_one();
}

cycle_state rack::cycle(cycle_type type) {

	auto state = cycle_state::empty;

	for( auto& wptr : m_mainlines ) {
		auto u = wptr.second.lock();
		if(!u) continue;
		state = u->cycle_line(type);
	}

	return state;
}

void rack::sync(sync_flag flags) {

	if((flags & (sync_flag)sync_flags::sync_duration)) {
		return profile_sync(flags);
	}

	if((flags & (sync_flag) sync_flags::glob_sync)) {
		sync(m_global_profile, (sync_flag)sync_flags::none);
		sync(m_global_profile, flags);
		sync_cache();
	}

	cycle(cycle_type::sync);

}

void rack::sync(sync_profile& profile, sync_flag flags) {
	for(auto& wptr : m_mainlines) {
			auto u = wptr.second.lock();
			if(!u) continue;
			u->sync_line(profile, flags);
	}
}

void rack::profile_sync(sync_flag flags) {
	pclock::time_point t_start, t_end;

	t_start = pclock::now();
	cycle(cycle_type::sync);
	t_end = pclock::now();

	m_rack_profile.sync_duration = std::chrono::duration_cast<profile_duration>(t_end-t_start);
}

void rack::sync_cache() {
	if(m_cache == nullptr) return;
	if(m_cache->block_size() > 0) return;
	
	m_cache->build_cache(m_global_profile.period * m_global_profile.channels);
}

bool rack::profile_line(sync_profile& profile, std::string mainline) {
	auto it = m_mainlines.find(mainline);

	if(it == m_mainlines.end()) return false;
	if(it->second.expired()) return false;
	auto shr = it->second.lock();
	shr->sync_line(profile, 0);
	return true;
}

const rack_profile & rack::profile() {
	return m_rack_profile;
}

const sync_profile & rack::global_profile() {
	return m_global_profile;
}

midi::midi_handler* rack::get_midi_handler() {
	return m_midi;
}

void rack::set_midi_handler(midi::midi_handler* midi) {
	m_midi = midi;
}

void rack::warmup() {
	cycle(cycle_type::warmup);
	cycle(cycle_type::sync);
	sync((sync_flag)sync_flags::glob_sync);
}

void rack::start() {
	m_running = true;
	m_active = false;

	m_rack_thread = std::thread([this](){

		m_active = true;
		std::unique_lock<std::mutex> lock(m_trigger_mutex);

		while(m_running) {
			m_trigger.wait(lock);
			if(!m_running) {
				lock.unlock();
				break;
			}
			cycle();
			if(m_resync) {

				if(m_resync_flags) {
					sync(m_resync_flags);
				} else {
					cycle(cycle_type::sync);
				}
			}

		}
		m_active = false;
	});

	// busy loop until activation
	while(!m_active) {
		asm(""); //  don't optimise away loop!
		continue;
	}
}

void rack::stop() {
	m_running = false;
	m_trigger.notify_one();
	m_rack_thread.join();
}

bool rack::active() {
	return m_active;
}

bool rack::running() {
	return m_running;
}
