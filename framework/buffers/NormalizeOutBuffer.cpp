#include "NoramlizeOutBuffer.h"

template<typename T>
NormalizeOutBuffer<T>::NormalizeOutBuffer(int size) {
	buffer = malloc(sizeof(T)*size);
	normal = 0;
	threshold = 0;
	capacity = size;
}

template<typename T>
NormalizeOutBuffer<T>::NormalizeOutBuffer(int size, int outNormal) {
	buffer = malloc(sizeof(T)*size);
	normal = outNormal;
	threshold = normal>>1;
	capacity = size;
}

template<typename T>
void NormalizeOutBuffer<T>::setNormalOut(int outNormal) {
	normal = outNormal;
	threshold = normal>>1;
}

template<typename T>
int NormalizeOutBuffer<T>::getNormalOut() {
	return normal;
}

template<typename T>
void NormalizeOutBuffer<T>::setThreshold(int value) {
	threshold = value;
}

template<typename T>
void NormalizeOutBuffer<T>::getThreshold() {
	return threshold;
}

template<typename T>
int NormalizeOutBuffer<T>::getLoadline() {
	return loadline;
}

template<typename T>
int NormalizeOutBuffer<T>::getCapacity() {
	return capacity;
}

template<typename T>
int NormalizeOutBuffer<T>::writeReady(int size) {
	if(loadline+size > capacity)
		return false;

	return true;
}

template<typename T>
int NormalizeOutBuffer<T>::readReady() {
	if(loadline > threshold && (loadline-threshold) > normal)
		return true

	return false;
}
