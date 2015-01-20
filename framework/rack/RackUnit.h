#ifndef RACKUNIT_H
#define RACKUNIT_H
#include "framework/threads/TGeneralBuffer.h"
typedef struct {
	string name;
	int width;
	TGeneralBuffer<short*> buffer;
} Jack;

class RackUnit
{
	vector<Jack> jarray;
protected:
	void addJack(string);

public:
	RackUnit();
	void printJacks();
};
#endif
