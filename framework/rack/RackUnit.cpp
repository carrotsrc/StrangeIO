#include "RackUnit.h"
RackUnit::RackUnit() {
	unitState = UNIT_OFF;
}

RackUnit::RackUnit(string uname) {
	name = uname;
	unitState = UNIT_OFF;
}


void RackUnit::setName(string uname) {
	name = uname;
}

string RackUnit::getName() const {
	return name;
}

void RackUnit::setChain(RackChain *rchain) {
	chain = rchain;
}

RackChain *RackUnit::getChain() const {
	return chain;
}

void RackUnit::addJack(string jname, ConnectorType type) {
	Jack *jack;

	switch(type) {
	case JACK_AC:
		jack = (Jack*)new AcJack(this);
		break;
	case JACK_THREADED:
		jack = (Jack*)new ThreadedJack(this);
		break;
	case JACK_SEQ:
		jack = (Jack*)new SeqJack(this);
		break;
	}

	jack->name = jname;
	jackArray.push_back(jack);
}

void RackUnit::addPlug(string pname) {
	Plug *plug = new Plug(this);
	plug->name = pname;
	plugArray.push_back(plug);
}

void RackUnit::printJacks() {
	int sz = jackArray.size();
	for(int i = 0; i < sz; i++)
		cout << jackArray[i]->name << endl;
}

Jack *RackUnit::getJack(string name) const {
	int sz = jackArray.size();
	for(int i = 0; i < sz; i++)
		if(jackArray[i]->name == name)
			return (jackArray[i]);

	return NULL;
}

Plug *RackUnit::getPlug(string name) const {
	int sz = plugArray.size();
	for(int i = 0; i < sz; i++)
		if(plugArray[i]->name == name)
			return (plugArray[i]);

	return NULL;
}

void RackUnit::join() {

}

void RackUnit::unjoin() {

}

RackState RackUnit::rackFeed(RackState state) {
	switch(state) {
	case RACK_AC:
		if(unitState == UNIT_ACTIVE)
			break;
	case RACK_RESET:
		if(init() == RACK_UNIT_FAILURE)
			return RACK_UNIT_FAILURE;

		unitState = UNIT_ACTIVE;
		break;
	
	case RACK_OFF:
		unitState = UNIT_OFF;
		break;
	}

	int sz = plugArray.size();
	for(int i = 0; i < sz; i++) {
		if(plugArray[i]->connected &&
		plugArray[i]->jack->rackFeed(state) == RACK_UNIT_FAILURE)
			return RACK_UNIT_FAILURE;
	}

	return RACK_UNIT_OK;
}

void RackUnit::setConnection(string plug, string jack, RackUnit *unit) {
	Plug *p = this->getPlug(plug);
	Jack *j= unit->getJack(jack);
	p->jack = j;
	p->connected = true;
	j->connected = true;
}
