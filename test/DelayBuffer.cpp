#include "framework/buffers/DelayBuffer.h"
#include "common.h"

void dprint(const int *d) {
	cout << "---" << endl;
	for(int i = 0; i < 8; i++)
		cout << d[i] << " ";

	cout << endl;
}

int main( void ) {
	RackoonIO::DelayBuffer<int> buffer(8);
	int testA[22], testB[2];
	int j = 1;
	for(int i = 0; i < 22; i++)
		testA[i] = j++;


	RackoonIO::DelayBufferState cState = RackoonIO::DelayBufferState::OK;
	if(buffer.supply(testA, 22) == RackoonIO::DelayBufferState::OK)
		cout << "testA OK" << endl;
	else
		cout << "testA WAIT" << endl;

	while(cState != RackoonIO::DelayBufferState::WAIT) {
		for(int i = 0; i < 2; i++)
			testB[i] = j++;

		cState = buffer.supply(testB, 2);
	}
	cout << "Full on j: " << j << endl;
	const int *flush = buffer.flush();
	dprint(flush);
	cout << "Done!" << endl;

}
