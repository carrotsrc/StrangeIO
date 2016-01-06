#include <string>
#include "framework/component/registrable.hpp"

using namespace strangeio::component;

registrable::registrable(std::string model, std::string label, ctype type)
	: m_model(model), m_label(label)
	, m_type(type)
{ }

registrable::~registrable() {
}

void registrable::set_id(long id) {
	m_component_id = id;
}

void registrable::set_handle(rhandle handle) {
	m_handle = handle;
}

rhandle registrable::handle() {
	return m_handle;
}

std::string registrable::umodel() const {
	return m_model;
}

std::string registrable::ulabel() const {
	return m_label;
}

ctype registrable::type() const {
	return m_type;
}

long registrable::sysid() const {
	return m_component_id;
}

