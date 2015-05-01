#include "framework/common.h"
#include "framework/hosting/lv2/LV2Platform.h"
using namespace std;

int main( void ) {
	RackoonIO::Hosting::LV2Platform platform;
	string uri = "http://plugin.org.uk/swh-plugins/amp";
	auto plugin = platform.getPlugin(uri);
	if(plugin == nullptr) {
		cerr << "Failed to load plugin " << uri << endl;
	}

	cout << plugin->getNumPorts() << endl;
	cout << plugin->getUri() << endl;
}
