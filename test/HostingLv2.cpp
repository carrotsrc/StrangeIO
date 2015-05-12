#include <math.h>
#include "HostingLv2.h"

using namespace std;

#define FS 512

PcmSample *generate_sample_array(uint32_t fs) {
	return (PcmSample*) malloc(sizeof(PcmSample)*fs);
}

PcmSample *generate_sine(uint32_t fs) {
	auto ts = 1.0f/fs;
	auto in = generate_sample_array(fs);

	for(int i = 0; i < 512; i++) {
		in[i] = sin(2 * M_PI * ts * i);
	}

	return in;
}

int main( void ) {
	RackoonIO::Hosting::LV2Platform platform;
	auto factory = new TestFactory();

	factory->setLV2Platform(&platform);


	auto unit = factory->build("TestAdaptor", "TestAdaptor1");


	auto in = generate_sine(FS);
	auto out = generate_sample_array(FS);
	auto gain = 2.0f;


	auto jack = unit->getJack("Input");
	cout << jack->name << endl;

	for(int i = 0; i < 512; i++) {
		cout << in[i] << "\t" << out[i] << endl;
	}
	cout << "Successful" << endl;
}
