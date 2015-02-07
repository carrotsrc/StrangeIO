#include "NoramlizeOutBuffer.h"

template<typename T>
NormalizeOutBuffer<T>::NormalizeOutBuffer(int size) {
	buffer = malloc(sizeof(T)*size);
	normal = 0;
	threshold = 0;
}

template<typename T>
NormalizeOutBuffer<T>::NormalizeOutBuffer(int size, int outNormal) {
	buffer = malloc(sizeof(T)*size);
	normal = outNormal;
	threshold = normal>>1;
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
void NormalizeOutBuffer<T>::getWaterline() {
	return waterline;
}

template<typename T>
int NormalizeOutBuffer<T>::readyWrite() {
}

template<typename T>
int NormalizeOutBuffer<T>::readRead() {
