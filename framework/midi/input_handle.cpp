#include "framework/midi/input_handle.hpp"

using namespace strangeio::midi;
input_handle::input_handle(std::string port_name) :
m_port_name(port_name)
{ }

std::string input_handle::port_name() {
	return m_port_name;
}