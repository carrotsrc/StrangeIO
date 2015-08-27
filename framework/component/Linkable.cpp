#include "framework/component/Linkable.hpp"

using namespace StrangeIO::Component;

Linkable::Linkable() :
m_num_inputs(0), m_num_outputs(0)
{ }

bool Linkable::connect(int id, LinkIn* in) {
	if( (unsigned int) id >= m_num_inputs) return false;
	if(in == nullptr) return false;

	m_outputs[id].to = in;
	m_outputs[id].connected = true;
	m_outputs[id].to->connected = true;

	return true;
}

void Linkable::disconnect(int id) {
	if( (unsigned int) id >= m_num_inputs) return;

	m_outputs[id].connected = false;
	m_outputs[id].to->connected = false;
	m_outputs[id].to = nullptr;

}

const int Linkable::has_input(std::string label) const {
	for(auto& in : m_inputs) {
		if(in.label == label) {
			return in.id;
		}
	}

	return -1;
}

const LinkIn* Linkable::get_input(int id) const {
	if( (unsigned int) id >= m_num_inputs) return nullptr;

	return &m_inputs[id];
}

const int Linkable::has_output(std::string label) const {
	for(auto& out : m_outputs) {
		if(out.label == label) {
			return out.id;
		}
	}

	return -1;
}

const LinkOut* Linkable::get_output(int id) const {
	if( (unsigned int) id >= m_num_outputs) return nullptr;

	return &m_outputs[id];
}


void Linkable::add_output(std::string label) {

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

void Linkable::add_input(std::string label) {

	m_inputs.push_back(
				LinkIn{
					.label = label,
					.id = m_num_inputs,
					.unit = this,
					}
			);

	++m_num_inputs;
}

bool Linkable::feed_out(PcmSample* samples, int id) {

	if( (unsigned int)id >= m_num_outputs) return false;

	auto& out = m_outputs[id];

	if(!out.connected) return false;

	out.to->unit->feed_line(samples, out.to->id);
	return true;
}

const std::vector<LinkIn> & Linkable::inputs() const {
	return m_inputs;
}

const std::vector<LinkOut> & Linkable::outputs() const {
	return m_outputs;
}

bool Linkable::input_connected(unsigned int id) const {
	if(id > m_num_inputs) return false;

	return m_inputs[id].conncted;
}

bool Linkable::output_conncted(int id) const {
	if(id > m_num_outputs) return false;

	return m_outputs[id].conncted;
}
