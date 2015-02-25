/*
 * Copyright (C) 2015  charlie <charlie@carrotsrc.org>
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
enum DelayBufferState {
	OK,
	WAIT
}
template<typename T>
class DelayBuffer
{
public:
	DelayBuffer(int);
	DelayBufferState supply(T*, int);
	T *flush();

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
DelayBufferState
DelayBuffer<T>::supply(T *period, int pSize) {
	(load + pSize > bSize)
		return WAIT;
}

#endif // DELAYBUFFER_H
