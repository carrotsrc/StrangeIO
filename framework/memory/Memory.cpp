/*
 * <one line to give the library's name and an idea of what it does.>
 * Copyright (C) 2015  charlie <charlie@carrotsrc.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3.0 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include "Memory.h"
using namespace RackoonIO;
Memory::Memory() {

}

void Memory::init (int bSize, int nBlocks) {
	int sbit = numBlocks/8;
	blockSize = bSize;
	numBlocks = nBlocks;
	freeBlocks = (char*) calloc(sbit, sizeof(char));
	
	blocks = (short*) calloc(numBlocks*blockSize, sizeof(short));
	first = blocks;
	last = blocks+(blockSize*numBlocks);
	mid = first + ((numBlocks/2)*blockSize);
}

short *Memory::alloc(int num) {
	int block, bit;
	for(block = 0; block < numBlocks; block++) {
		if(freeBlocks[block] < 0xff) {
			for(bit = 0; bit < 8; bit++) {
				if(!((1<<bit)&freeBlocks[block]))
					break;
			}
			break;
		}

	}
	short *mem = blocks+((block*blockSize)+bit);

}
