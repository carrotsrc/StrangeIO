#include "RackUnit.h"
RackUnit::RackUnit() {

}

void RackUnit::addJack(string name) {
	jarray.push_back({.name = name});
}

void RackUnit::printJacks() {
	int sz = jarray.size();
	for(int i = 0; i < sz; i++)
		cout << jarray[i].name << endl;
}
