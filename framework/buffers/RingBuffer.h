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
#include <cstdlib>

namespace RackoonIO {

namespace Buffers {

/** An implementation of a ring/circular buffer
 *
 * The ring buffer is a block of memory that
 * effectively loops back to the beginning when
 * it reachs the end; super effective if you
 * are dealing with a stream.
 *
 * Writing increments the write index; reading
 * increments the read index;
 *
 * If the input exceeds the output then
 * data will start getting overwritten by the
 * new data
 */
template<typename T>
class RingBuffer
{
	T *buffer; ///< The buffer

	short size; ///< The size of the buffer
	short width; ///< The size of individual items in the buffer
	short count; ///< Current load
	short start; ///< The starting index
	short end; ///< The end index


public:
	/** Initialiase the buffer with specified space
	 *
	 * @param bsize The capacity of the buffer in terms of data type (*not* bytes)
	 */
	RingBuffer(short bsize);

	/** Get the size of the buffer
	 *
	 * @return The current size
	 */
	int getSize();

	/** Resize the buffer
	 *
	 * @param bsize New capacity of buffer
	 * @return true on resize; otherwise false
	 */
	bool resize(short bsize);

	/** get current load
	 *
	 * @return The number of items in buffer
	 */
	short getCount();

	/** Add an item to the buffer - increment write index
	 *
	 * @param data The item to add
	 */
	void add(T data);

	/** Read an item from the buffer - increment read index
	 *
	 * @return The data read
	 */
	T read();
};

// Template definition

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
