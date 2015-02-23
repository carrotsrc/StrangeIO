 /* Copyright (C) 2015  Charlie Fyvie-Gauld
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
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

#include "BitfieldMemory.h"
using namespace RackoonIO;

BitfieldMemory::BitfieldMemory() {

}

void BitfieldMemory::init (int bSize, int nBlocks) {
	int sbit = numBlocks/8;
	blockSize = bSize;
	numBlocks = nBlocks;
	freeBlocks = (char*) calloc(sbit, sizeof(char));
	
	blocks = (short*) calloc(numBlocks*blockSize, sizeof(short));
	first = blocks;
	last = blocks+(blockSize*numBlocks);
	mid = first + ((numBlocks/2)*blockSize);
	cout << first << endl;
}

short *BitfieldMemory::alloc(int num) {
	int byte, bit;
	for(byte = 0; byte < numBlocks; byte++) {
		if(freeBlocks[byte] < 0xff) {
			for(bit = 0; bit < 8; bit++) {
				if(!((1<<bit)&freeBlocks[byte])) {
					freeBlocks[byte] |= (1<<bit);
					break;
				}
			}
			break;
		}

	}
	short *mem = blocks + (((byte<<3)+bit)*(blockSize*sizeof(short)));
	return mem;
}

void BitfieldMemory::__print_state() {
	int byte, bit, nchunk;
	nchunk = numBlocks>>3;

	for(byte = 0; byte < nchunk; byte++) {
		for(bit = 0; bit < 8; bit++) {
			if(!((1<<bit)&freeBlocks[byte])) {
				cout << "-";
			} else {
				cout << "+";
			}
		}

	}
	cout << endl << endl; 
}

void BitfieldMemory::free(short *mem) {
	int block, byte, bit;
	block = ((int)(mem - first)/sizeof(short))/blockSize;

	byte = block>>3;
	bit = block-(byte<<3);
	freeBlocks[byte] ^= 1<<bit;
}



