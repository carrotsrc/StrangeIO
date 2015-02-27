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
#ifndef TRINGBUFFER_H
#define TRINGBUFFER_H
#include "common.h"

namespace RackoonIO {

namespace Buffers {

template<typename T>
class RingBuffer
{
	T *buffer;

	short size;
	short width;
	short count;
	short start;
	short end;


public:
	RingBuffer(short);

	int getSize();
	bool resize(short);

	short getCount();

	void add(T);
	T read();
};

template<typename T>
RingBuffer<T>::RingBuffer(short bsize) {
	size = bsize;
	width = sizeof(T);
	buffer = (T*)calloc(size, width);
}

template<typename T>
int RingBuffer<T>::getSize() {
	return size;
}

template<typename T>
bool RingBuffer<T>::resize(short bsize) {
	size = bsize;
	T *tmp = (T*)realloc(buffer, width*size);
	if(!tmp)
		return false;

	buffer = tmp;
	return true;
}

template<typename T>
short RingBuffer<T>::getCount() {
	return count;
}

template<typename T>
void RingBuffer<T>::add(T data) {
	end = (start+count)%size;
	buffer[end] = data;
	if(count == size)
		start = (start+1)%size;
	else
		count++;
}

template<typename T>
T RingBuffer<T>::read() {
	T e = buffer[start];
	start = (start+1)%size;
	count--;
}

}// Buffers
}// RackoonIO
#endif
