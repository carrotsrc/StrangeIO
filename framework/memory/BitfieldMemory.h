/*
 * Copyright (C) 2015  Charlie Fyvie-Gauld <charlie@carrotsrc.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef BITFIELDMEMORY_H
#define BITFIELDMEMORY_H
#include "Memory.h"
namespace RackoonIO {
	
class BitfieldMemory : public Memory
{
public:
	BitfieldMemory();
	void init(int, int);
	short *alloc(int);

	void __print_state();

private:
	char *freeBlocks;
	short *blocks;
	short *first, *mid, *last;
};

}

#endif // BITFIELDMEMORY_H
