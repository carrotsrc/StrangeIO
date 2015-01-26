#ifndef TGENERALBUFFER_H
#define TGENERALBUFFER_H
#include "framework/threads/MutexLock.h"
namespace RackoonIO {

template<typename Type>
class TGeneralBuffer : public MutexLock
{
	Type buffer;
	short size;
	bool full;
public:
	TGeneralBuffer();
	bool isFull();

	void add(Type);
	Type read();
};

}
#endif 
