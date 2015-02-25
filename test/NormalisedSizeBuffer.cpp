#include "framework/buffers/NormalisedSizeBuffer.h"
#include "common.h"

void dprint(int *d) {
	for(int i = 0; i < 16; i++)
		cout << d[i] << " ";

	cout << endl;
}
int main( void ) {
	RackoonIO::NormalisedSizeBuffer<int> buffer(16, 16<<2);
	int testA[22], testB[27];
	for(int i = 0; i < 22; i++)
		testA[i] = i+1;

	for(int i = 0; i < 22; i++)
		testA[i] = i+23;

	int *dispatch;

	NormalisedSizeBufferState cState = DISPATCH;


	while(cState != PARTIAL) {
		// First check
		if(buffer.getState() == DISPATCH) {
			dispatch = buffer.dispatch();
			dprint(dispatch);
		}
	}

}
