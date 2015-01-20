#include "TGeneralBuffer.h"

template<typename Type>
TGeneralBuffer<Type>::TGeneralBuffer() {
	full = false;
}

template<typename Type>
bool TGeneralBuffer<Type>::isFull() {
	return full;
}


template<typename Type>
void TGeneralBuffer<Type>::add(Type data) {
	buffer = data;
	full = true;
}

template<typename Type>
Type TGeneralBuffer<Type>::read() {
	full = false;
	return buffer;
}

template class TGeneralBuffer<short*>;
