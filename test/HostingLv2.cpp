#include <typeinfo>
#include <math.h>
#include "framework/common.h"
#include "framework/hosting/lv2/LV2Platform.h"
using namespace std;

int main( void ) {
	RackoonIO::Hosting::LV2Platform platform;
	string uri = "http://plugin.org.uk/swh-plugins/amp";
	auto plugin = platform.getPlugin(uri);
	if(plugin == nullptr) {
		cerr << "Failed to load plugin " << uri << endl;
		exit(EXIT_FAILURE);
	}

	if(!plugin->instantiate()) {
		cerr << "Failed to instantiate " << uri << endl;
		exit(EXIT_FAILURE);
	}
	plugin->init();
	auto ts = 1.0f/512.0f;
	auto in = (float*) malloc(sizeof(float)*512);
	auto out = (float*) malloc(sizeof(float)*512);
	auto gain = 2.0f;

	auto ports = plugin->getPortsOfType(RackoonIO::Hosting::LV2Port::Input);
	std::vector<const RackoonIO::Hosting::LV2Port*>::const_iterator it;

	for(int i = 0; i < 512; i++) {
		in[i] = sin(2 * M_PI * ts * i);
	}

	plugin->connectPort("Input", (void*)in);
	plugin->connectPort("Amps gain (dB)", &gain);
	plugin->connectPort("Output", (void*)out);
	plugin->run(512);

	for(int i = 0; i < 512; i++) {
		cout << in[i] << "\t" << out[i] << endl;
	}

	cout << "Successful" << endl;
}
