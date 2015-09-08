#include <memory>
#include "framework/routine/system.hpp"
#include "framework/component/unit_loader.hpp"
#include "framework/config/document.hpp"

#include "framework/thread/queue.hpp"
#include "framework/midi/driver_utility.hpp"
#include "framework/midi/midi_handler.hpp"
#include "framework/event/loop.hpp"
#include "framework/memory/cache_manager.hpp"

using namespace strangeio;

strangeio::component::rack_uptr 
strangeio::routine::system::setup(config::assembler& as, std::string config_path, int cache_blocks) {
	as.set_builder(std::unique_ptr<component::unit_loader>(new component::unit_loader()));

	config::document doc;

	auto vqueue = new thread::queue(2);
	auto vdriver = new midi::driver_utility();
	auto vloop = new event::loop();
	auto vcache = new memory::cache_manager(cache_blocks);
	auto vmidi = new midi::midi_handler(vdriver);

	auto sys = new component::rack();
	sys->set_queue_utility(vqueue);
	sys->set_loop_utility(vloop);
	sys->set_midi_handler(vmidi);
	sys->set_cache_utility(vcache);

	const auto vconfig = doc.load(config_path);
	as.assemble((*vconfig), *sys);
	vqueue->start();
	vmidi->init();
	vmidi->start();

	// Wait for queue to start
	std::this_thread::sleep_for(std::chrono::milliseconds(20));
	return component::rack_uptr(sys);
}