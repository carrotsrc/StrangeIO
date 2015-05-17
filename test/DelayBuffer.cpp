#include "framework/buffers/DelayBuffer.h"
#include "framework/common.h"

void dprint(const int *d) {
	std::cout << "---" << std::endl;
	for(int i = 0; i < 8; i++)
		std::cout << d[i] << " ";

	std::cout << std::endl;
}

int main( void ) {
	RackoonIO::Buffers::DelayBuffer<int> buffer(8);

	int testA[22], testB[2];
	int j = 1;
	for(int i = 0; i < 22; i++)
		testA[i] = j++;


	RackoonIO::Buffers::DelayBuffer<int>::State cState = RackoonIO::Buffers::DelayBuffer<int>::OK;
	if(buffer.supply(testA, 22) == RackoonIO::Buffers::DelayBuffer<int>::OK)
		std::cout << "testA OK" << std::endl;
	else
		std::cout << "testA WAIT" << std::endl;

	while(cState != RackoonIO::Buffers::DelayBuffer<int>::WAIT) {
		for(int i = 0; i < 2; i++)
			testB[i] = j++;

		cState = buffer.supply(testB, 2);
	}
	std::cout << "Full on j: " << j << std::endl;
	const int *flush = buffer.flush();
	dprint(flush);
	std::cout << "Done!" << std::endl;

}
