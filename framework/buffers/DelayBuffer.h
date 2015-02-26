/*
 * Copyright (C) 2015  Charlie Fyvie-Gauld
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DELAYBUFFER_H
#define DELAYBUFFER_H
#include "common.h"

namespace RackoonIO {



template<typename T>
class DelayBuffer
{
public:
	enum State {
		OK,
		WAIT
	};
	typedef State E;
	DelayBuffer(int);
	State supply(const T*, int);
	const T* flush();
	int getLoad();

private:
	int bSize, load;
	T *buffer;
};

// Template Defintions

template<typename T>
DelayBuffer<T>::DelayBuffer(int size) {
	bSize = size;
	buffer = (T*) malloc(size * sizeof(T));
	load = 0;
}

template<typename T>
int DelayBuffer<T>::getLoad() {
	return load;
}

template<typename T>
typename DelayBuffer<T>::State
DelayBuffer<T>::supply(const T *period, int pSize) {
	if(load + pSize > bSize)
		return WAIT;

	memcpy(buffer+load, period, pSize * sizeof(T));
	load += pSize;
	return OK;
}

template<typename T>
const T* DelayBuffer<T>::flush() {
	load = 0;
	return buffer;
}

} // RackoonIO
#endif // DELAYBUFFER_H
