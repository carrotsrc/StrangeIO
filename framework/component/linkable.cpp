#include "framework/component/linkable.hpp"

using namespace strangeio::component;

linkable::linkable() :
m_num_inputs(0), m_num_outputs(0)
{ }

bool linkable::connect(int id, LinkIn* in) {

	if( (unsigned int) id >= m_num_outputs) return false;
	if(in == nullptr) return false;


	m_outputs[id].to = in;
	m_outputs[id].connected = true;
	m_outputs[id].to->connected = true;

	return true;
}

void linkable::disconnect(int id) {
	if( (unsigned int) id >= m_num_inputs) return;

	m_outputs[id].connected = false;
	m_outputs[id].to->connected = false;
	m_outputs[id].to = nullptr;

}

int linkable::has_input(std::string label) const {
	for(const auto& in : m_inputs) {
		if(in.label == label) {
			return in.id;
		}
	}

	return -1;
}

const LinkIn* linkable::get_input(int id) const {
	if( (unsigned int) id >= m_num_inputs) return nullptr;

	return &m_inputs[id];
}

int linkable::has_output(std::string label) const {
	for(const auto& out : m_outputs) {
		if(out.label == label) {
			return out.id;
		}
	}

	return -1;
}

const LinkOut* linkable::get_output(int id) const {
	if( (unsigned int) id >= m_num_outputs) return nullptr;

	return &m_outputs[id];
}


void linkable::add_output(std::string label) {

	m_outputs.push_back(
				LinkOut{ 
					.label = label,
					.id = m_num_outputs,
					.to = nullptr,
					.connected = false
					}
			);

	++m_num_outputs;
}

void linkable::add_input(std::string label) {

	m_inputs.push_back(
				LinkIn{
					.label = label,
					.id = m_num_inputs,
					.unit = this,
					}
			);

	++m_num_inputs;
}

bool linkable::feed_out(memory::cache_ptr samples, int id) {

	if( (unsigned int)id >= m_num_outputs) return false;

	auto& out = m_outputs[id];

	if(!out.connected) return false;

	out.to->unit->feed_line(samples, out.to->id);
	return true;
}

bool linkable::fill_out(memory::cache_ptr samples, int id) {

	if( (unsigned int)id >= m_num_outputs) return false;

	auto& out = m_outputs[id];

	if(!out.connected) return false;

	out.to->unit->fill_line(samples, out.to->id);
	return true;
}

const std::vector<LinkIn> & linkable::inputs() const {
	return m_inputs;
}

const std::vector<LinkOut> & linkable::outputs() const {
	return m_outputs;
}

bool linkable::input_connected(unsigned int id) const {
	if(id > m_num_inputs) return false;

	return m_inputs[id].connected;
}

bool linkable::output_conncted(unsigned int id) const {
	if(id > m_num_outputs) return false;

	return m_outputs[id].connected;
}
