#include <iostream>
#include "framework/memory/BitfieldMemory.h"

int main( void ) {
	cout << "Testing memory manager" << endl;
	short *blockA, *blockB;
	RackoonIO::BitfieldMemory memManager;

	memManager.init(512, 150);
	memManager.__print_state();

	blockA = memManager.alloc(1);
	memManager.__print_state();
	
	blockB = memManager.alloc(1);
	memManager.__print_state();
}
