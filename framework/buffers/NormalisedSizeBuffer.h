/* Copyright 2015 Charlie Fyvie-Gauld
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published 
 *  by the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef NORMALISEDSIZEBUFFER_H
#define NORMALISEDSIZEBUFFER_H
#include "common.h"

namespace RackoonIO {

enum NormalisedSizeBufferState {
	DISPATCH,
	PARTIAL,
	OVERFLOW
};

template<typename T>
class NormalisedSizeBuffer {
public:
	NormalisedSizeBuffer(int, int);
	NormalisedSizeBufferState supply(T*, int);
	T *dispatch();

	NormalisedSizeBufferState getState();

private:
	int load;
	const int nSize;

	T *buffer, *remainder, *rRead, *rWrite, *bound;
	NormalisedSizeBufferState state;
};

/*
 * Template definitions
 */

template<typename T>
NormalisedSizeBuffer<T>::NormalisedSizeBuffer ( int normal, int overflow ) {
	nSize = normal;
	buffer = (T*) malloc(sizeof(T)*nSize);
	remainder = (T*) malloc(sizeof(T)*nSize+overflow);
	bound = remainder + (nSize+overflow);
	rWrite = rRead = remainder;
	load = 0;
	state = PARTIAL;
}

template<typename T>
NormalisedSizeBufferState
NormalisedSizeBuffer<T>::supply (T* period, int pSize) {
	if(rWrite < rRead && (rWrite + pSize) > rRead)
		return OVERFLOW;

	int cpy = pSize;
	T *pRead = period;

	if(rWrite + pSize >= bound)
		cpy = bound - rWrite;

	memcpy(rWrite, pRead, cpy*sizeof(T));
	rWrite += cpy;
	pRead += cpy;

	if((cpy = (pSize-cpy) > 0)) {
		rWrite = remainder;
		memcpy(rWrite, pRead, cpy*sizeof(T));
		rWrite += cpy;
	}

	load += pSize;

	if(load >= nSize)
		state = DISPATCH;
	else
		state = PARTIAL;

	return state;
}

template<typename T>
T *NormalisedSizeBuffer<T>::dispatch () {
	if(state == PARTIAL)
		return nullptr;

	int cpy = nSize;
	T* bWrite = buffer;

	if(rRead + nSize > bound) {
		cpy = bound - rRead;
		memcpy(bWrite, rRead, cpy * sizeof(T));
		rRead = remainder;
		bWrite += cpy;
		cpy = nSize - cpy;
	}

	memcpy(bWrite, rRead, cpy * sizeof(T));
	load -= nSize;
	return buffer;
}

template<typename T>
NormalisedSizeBufferState
NormalisedSizeBuffer<T>::getState () {
	return state;
}

} // RackoonIO

#endif
