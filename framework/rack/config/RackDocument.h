#ifndef __RACKDOCUMENT_H_1439199800__
#define __RACKDOCUMENT_H_1439199800__
 
#include "framework/common.h"
#include "framework/picojson/picojson.h"
#include "RackDesc.h"

namespace StrangeIO {
namespace Config {

class RackDocument {
public:
	enum Element {
		Root,
		System,
	};

	std::unique_ptr<RackDesc> load(std::string path);

private:
	std::unique_ptr<RackDesc> mRack;
	std::string loadFile(std::string);
	void parseDocument(const picojson::value& v, RackDocument::Element element);
	void parseRack(const picojson::value& v);
	void parseUnit(std::string label, const picojson::value& v);
	void parseBindings(RackDesc::Setup::Unit& unit, const picojson::value & v);
};

} // Config
} // StrangeIO
#endif
