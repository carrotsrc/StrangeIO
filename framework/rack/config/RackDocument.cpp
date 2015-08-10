#include <fstream>
#include "RackDocument.h"

using namespace StrangeIO::Config;

using Pval = picojson::value;
using Pobj = picojson::object;
using Pnull = picojson::null;
using Parr = picojson::array;

std::unique_ptr<RackDesc> RackDocument::load(std::string path) {
	Pval v;
	std::string config = loadFile(path);
	auto err = picojson::parse(v, config);
	if(!err.empty())
		std::cout << err << std::endl;


	mRack = std::unique_ptr<RackDesc>(new RackDesc);
	parseDocument(v, Root);

	return std::move(mRack);
}

std::string RackDocument::loadFile(std::string path) {
	
	std::ifstream f(path);
	if(f.fail()) {
		std::cerr << "Config file `" << path <<"` not found!" << std::endl;
		exit(1);
	}
	std::string config((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
	f.close();
	return config;
}

void RackDocument::parseDocument(const Pval& v, RackDocument::Element element) {

	Pval cv;

	if(v.is<picojson::object>()) {
		auto& obj = v.get<Pobj>();
			for (auto& i : obj) {
				if(i.first == "system" && element == Root) {

					parseDocument(i.second, System);

				} else if(i.first == "rack" && element == Root) {

					parseRack(i.second);

				} else if(i.first == "midi" && element == Root) {

					for (auto& mit : i.second.get<Pobj>()) {
						mRack->midi.controllers.push_back({
							.label = mit.first,
							.port = mit.second.get<std::string>(),
						});
					}

				} else if(i.first == "threads" && element == System) {

					cv = i.second.get("workers");
					if(!cv.is<Pnull>()) {
						mRack->system.threads.num_workers = static_cast<int>(cv.get<double>());
					}

					return;
				}
		}
	}
}

void RackDocument::parseRack(const Pval& v) {
	std::string fm, pl, to, jk;

	Pval cv;

	// get the virtual mainlines plugs
	auto mlines = v.get("mainlines").get<Parr>();

	for(const auto & mit : v.get("mainlines").get<Parr>()) {
		mRack->setup.mainlines.push_back(mit.get("plug").get<std::string>());
	}

	// get the daisy chains
	for(const auto&  it : v.get("daisychains").get<Parr>()) {
		fm = it.get("from").get<std::string>();
		pl = it.get("plug").get<std::string>();
		to = it.get("to").get<std::string>();
		jk = it.get("jack").get<std::string>();

		mRack->setup.daisychains.push_back({
			.from = fm, .plug = pl, .to = to, .jack = jk
		});

		auto uv = v.get(to);
		if(uv.is<Pnull>())
			continue;

		parseUnit(to, uv);
	}
}

void RackDocument::parseUnit(std::string label, const Pval& v) {
	for(const auto& u : mRack->setup.units) {
		if(u.label == label) {
			return;
		}
	}

	auto cv = v.get("library");
	std::string target;
	RackDesc::Setup::Unit unit;

	if (!cv.is<Pnull>()) {
		unit.library = cv.get<std::string>();
	}

	cv = v.get("bindings");
	if (!cv.is<Pnull>()) {
		parseBindings(unit, cv);
	}

	cv = v.get("config");
	if(!cv.is<Pnull>()) {
		for(const auto & it : cv.get<Pobj>()) {
			unit.configs.push_back( {
				.type = it.first, 
				.value = it.second.get<std::string>()
			});
		}
	}
	mRack->setup.units.push_back(unit);

}

void RackDocument::parseBindings(RackDesc::Setup::Unit& unit, const Pval & v) {

	for (const auto & bit : v.get<Pobj>()) {
		if(bit.second.get("module").is<Pnull>() || bit.second.get("code").is<Pnull> ())
			continue;

		auto method = bit.first;
		auto module = bit.second.get("module").get<std::string>();
		auto code = static_cast<int>(bit.second.get("code").get<double>());

		unit.bindings.push_back({
			.name = method, .module = module, .code = code
		});
	}
}
