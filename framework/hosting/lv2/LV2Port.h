#ifndef LV2PORT_H
#define LV2PORT_H

namespace RackoonIO {
namespace Hosting {

struct LV2Port {
	enum PortType {
		Input,
		Output,
		Midi,
	};

	const std::string symbol;
	const PortType type;
	const uint32_t index;
	const LilvPort *port;
};


} // Hosting
} // RackoonIO

#endif
