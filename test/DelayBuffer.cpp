#include "framework/buffers/DelayBuffer.h"
#include "framework/common.h"

void dprint(const PcmSample *d) {
	std::cout << "---" << std::endl;
	for(int i = 0; i < 8; i++)
		std::cout << d[i] << " ";

	std::cout << std::endl;
}

int main( void ) {
	RackoonIO::Buffers::DelayBuffer buffer(8);

	PcmSample testA[22], testB[2];
	int j = 1;
	for(int i = 0; i < 22; i++)
		testA[i] = j++;


	auto cState = RackoonIO::Buffers::DelayBuffer::OK;
	if(buffer.supply(testA, 22) == RackoonIO::Buffers::DelayBuffer::OK)
		std::cout << "testA OK" << std::endl;
	else
		std::cout << "testA WAIT" << std::endl;

	while(cState != RackoonIO::Buffers::DelayBuffer::WAIT) {
		for(int i = 0; i < 2; i++)
			testB[i] = j++;

		cState = buffer.supply(testB, 2);
	}
	std::cout << "Full on j: " << j << std::endl;
	const PcmSample* flush = buffer.flush();
	dprint(flush);
	std::cout << "Done!" << std::endl;

}
