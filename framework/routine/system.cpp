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
//static int qos_fd = -1;
#if DEVBUILD
#include <cstdlib>
#include <csignal>


#ifdef __linux__
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#define SIO_DBG_INVOKE "gdb attach %d"
static void dbg_signal_handler(int signum) {
	char cmd[64];
	switch(signum) {
	case SIGSEGV:
		snprintf(cmd, 64, SIO_DBG_INVOKE, getpid());
		system(cmd);
		abort();
		break;
	case SIGINT:
		std::cout << "Terminating StrangeIO" << std::endl;
//		close(qos_fd);
		exit(0);
		break;
	}
}
#endif
#endif
strangeio::component::rack_uptr 
strangeio::routine::system::setup(config::assembler& as, std::string config_path, int cache_blocks) {
#if DEVBUILD
	signal(SIGSEGV, &dbg_signal_handler);
	signal(SIGINT, &dbg_signal_handler);
#endif
/*
	int32_t latency = 0;
	
	qos_fd = open("/dev/cpu_dma_latency", O_RDWR);
	if(qos_fd >= 0) {
		write(qos_fd, &latency, sizeof(latency));
		std::cout << "Successfully set cpu_dma_latency" << std::endl;
	} else {
		std::cout << "Error setting cpu_dma_latency" << std::endl;
	}
*/
	as.set_builder(std::unique_ptr<component::unit_loader>(new component::unit_loader()));

	config::document doc;

	auto vqueue = new thread::queue(2);
	auto vdriver = new strangeio::midi::driver_utility();
	auto vloop = new event::loop();
	auto vcache = new memory::cache_manager(cache_blocks);
	auto vmidi = new strangeio::midi::midi_handler(vdriver);

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