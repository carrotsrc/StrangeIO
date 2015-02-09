#ifndef NORMALIZEOUTBUFFER_H
#define NORMALIZEOUTBUFFER_H
#include "common.h"

namespace RackoonIO {

template<typename T>
class NormalizeOutBuffer {
private:

	T *buffer, *remainder;

	int capacity, normal, 
	    loadLine, remLine, dtsize;

public:
	NormalizeOutBuffer(int);
	NormalizeOutBuffer(int, int);

	void setNormalOut(int);
	int getNormalOut();

	int getLoadline();
	int getCapacity();

	bool readReady();
	bool writeReady(int);

	T *read();
	void write(T*,int);
};

/*
 * Template definitions
 */


template<typename T>
NormalizeOutBuffer<T>::NormalizeOutBuffer(int size) {
	buffer = (T*) malloc(sizeof(T)*size);
	normal = 0;
	capacity = size;
}

template<typename T>
NormalizeOutBuffer<T>::NormalizeOutBuffer(int size, int outNormal) {
	buffer = (T*) malloc(sizeof(T)*size);
	normal = outNormal;
	capacity = size;
	dtsize = sizeof(T);
}

template<typename T>
void NormalizeOutBuffer<T>::setNormalOut(int outNormal) {
	normal = outNormal;
	dtsize = sizeof(T);
}

template<typename T>
int NormalizeOutBuffer<T>::getNormalOut() {
	return normal;
}

template<typename T>
int NormalizeOutBuffer<T>::getLoadline() {
	return loadLine;
}

template<typename T>
int NormalizeOutBuffer<T>::getCapacity() {
	return capacity;
}

template<typename T>
bool NormalizeOutBuffer<T>::writeReady(int size) {
}

template<typename T>
bool NormalizeOutBuffer<T>::readReady() {
	if(buffer == nullptr)
		return false;
	return true;
}

template<typename T>
T* NormalizeOutBuffer<T>::read() {
	return buffer;
}

template<typename T>
void NormalizeOutBuffer<T>::write(T *data, int size) {
	if((size + remLine) >= normal) {
		int diff = normal-remLine, r = remLine;

		for(int i = 0; i < diff; i++)
			*(buffer+remLine+i) = *(data+i);

		remLine = size+remLine-normal;
		memcpy(remainder, data+(diff), dtsize*remLine);
	} else {
		buffer = nullptr;
		memcpy(remainder+remLine, data, dtsize*size);
	}
}

} // RackoonIO
#endif
