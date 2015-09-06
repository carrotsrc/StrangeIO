#ifndef SYSTEM_HPP__1441550908__
#define SYSTEM_HPP__1441550908__
#include "framework/fwcommon.hpp"
#include "framework/component/rack.hpp"
#include "framework/config/assembler.hpp"
namespace strangeio {
namespace routine {
namespace system {

	strangeio::component::rack_uptr setup(config::assembler& as, std::string config_path, int cache_blocks);

}
}
}
#endif