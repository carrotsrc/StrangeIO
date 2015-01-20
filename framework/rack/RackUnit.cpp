#include "RackUnit.h"
RackUnit::RackUnit() {

}

void RackUnit::addJack(string jname, int size) {
	Jack jack = { .name = jname, .frames = size};
	jackArray.push_back(jack);
}

void RackUnit::addPlug(string pname) {
	Plug plug = { .name = pname};
	plugArray.push_back(plug);
}

void RackUnit::printJacks() {
	int sz = jackArray.size();
	for(int i = 0; i < sz; i++)
		cout << jackArray[i].name << endl;
}

Jack *RackUnit::getJack(string name) {
	int sz = jackArray.size();
	for(int i = 0; i < sz; i++)
		if(jackArray[i].name == name)
			return &(jackArray[i]);

	return NULL;
}

Plug *RackUnit::getPlug(string name) {
	int sz = plugArray.size();
	for(int i = 0; i < sz; i++)
		if(plugArray[i].name == name)
			return &(plugArray[i]);

	return NULL;
}
