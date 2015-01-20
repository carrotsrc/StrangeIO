#ifndef TRINGBUFFER_H
#define TRINGBUFFER_H
#include "MutexLock.h"
template<typename Type>
class TRingBuffer : public MutexLock
{
	Type *buffer;

	short size;
	short width;
	short count;
	short start;
	short end;

public:
	TRingBuffer(short);

	int getSize();
	bool resize(short);

	short getCount();

	void add(Type);
	Type read();
};
#endif
