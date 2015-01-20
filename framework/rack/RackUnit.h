#ifndef RACKUNIT_H
#define RACKUNIT_H
#include "framework/threads/TGeneralBuffer.h"
typedef struct {
	string name;

	int frames;
	TGeneralBuffer<short*> buffer;
} Jack;

typedef struct {
	string name;
} Plug;

class RackUnit
{
	vector<Jack> jackArray;
	vector<Plug> plugArray;

protected:
	void addJack(string, int);
	void addPlug(string);

public:
	RackUnit();
	void printJacks();

	Jack *getJack(string);
	Plug *getPlug(string);
};
#endif
