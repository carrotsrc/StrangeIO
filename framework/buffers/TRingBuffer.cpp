#include "TRingBuffer.h"

template<typename Type>
TRingBuffer<Type>::TRingBuffer(short bsize)
: MutexLock() {
	size = bsize;
	width = sizeof(Type);
	buffer = (Type*)calloc(size, width);
}

template<typename Type>
int TRingBuffer<Type>::getSize() {
	return size;
}

template<typename Type>
bool TRingBuffer<Type>::resize(short bsize) {
	size = bsize;
	Type *tmp = (Type*)realloc(buffer, width*size);
	if(!tmp)
		return false;

	buffer = tmp;
	return true;
}

template<typename Type>
short TRingBuffer<Type>::getCount() {
	return count;
}

template<typename Type>
void TRingBuffer<Type>::add(Type data) {
	end = (start+count)%size;
	buffer[end] = data;
	if(count == size)
		start = (start+1)%size;
	else
		count++;
}

template<typename Type>
Type TRingBuffer<Type>::read() {
	Type e = buffer[start];
	start = (start+1)%size;
	count--;
}


template class TRingBuffer<unsigned short>;
template class TRingBuffer<short>;
template class TRingBuffer<short*>;
