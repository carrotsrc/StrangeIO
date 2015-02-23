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
#include "TRingBuffer.h"
using namespace RackoonIO;

template<typename Type>
TRingBuffer<Type>::TRingBuffer(short bsize) {
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
