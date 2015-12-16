#include "framework/midi/output_handle.hpp"

using namespace strangeio::midi;
output_handle::output_handle(std::string port_name) :
m_port_name(port_name)
{ }

std::string output_handle::port_name() {
	return m_port_name;
}