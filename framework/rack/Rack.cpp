/* Copyright 2015 Charlie Fyvie-Gauld
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published
 *  by the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <fstream>
#include "Rack.h"
#include "framework/events/FrameworkMessages.h"

using namespace StrangeIO;
Rack::Rack() {
	rackState = RACK_OFF;
	mCycleCount = 0;
}

void Rack::init() {
	initRackQueue();
	midiHandler.init();
}

void Rack::setConfigPath(std::string path) {
	configPath = path;
}

void Rack::initRackQueue() {
	if(!mRackQueue)
		throw;

	mRackQueue->init();
}

void Rack::start() {
	rackState = RACK_AC;
	mCycleThread = std::thread(&Rack::cycle, this);
	eventLoop.addEventListener(FwProcComplete, std::bind(&Rack::onCycleEvent, this, std::placeholders::_1));
	eventLoop.start();
	midiHandler.start();

	// warm up cycle
	std::cout << "Warm up cycle..." << std::endl;
	mCycleCount++;
	mCycleCondition.notify_one();
}

void Rack::cycle() {

	std::mutex m;
	std::unique_lock<std::mutex> ul(m);

	while(1) {
		if(mCycleCount == 0)
			mCycleCondition.wait(ul);

		RACK_TELEMETRY(metricUnitCycleStart, std::chrono::steady_clock::now());
		for(auto plug : plugArray) {

			if(!plug.connected)
				continue;

			plug.jack->rackFeed(RackState::RACK_AC);

		}
		RACK_TELEMETRY(metricUnitCycleEnd, std::chrono::steady_clock::now());
		mCycleCount--;
	}

}

Plug* Rack::getPlug(std::string name) {

	for (auto& plug : plugArray) {

		if(plug.name == name) {
			return &plug;
		}

	}

	return nullptr;

}

void Rack::initEvents(int numEvents) {
	eventLoop.initEvents(numEvents);
}

std::map<std::string, std::shared_ptr<RackUnit> > Rack::getUnits() {
	return rackChain.getUnits();
}

std::shared_ptr<RackUnit> Rack::getUnit(std::string name) {
	auto unit = mUnits.find(name);
	if(unit == mUnits.end())
		return nullptr;

	return unit->second;
}

EventLoop *Rack::getEventLoop() {
	return &eventLoop;
}

void Rack::onCycleEvent(std::shared_ptr<EventMessage> msg) {
	mCycleCount++;
	mCycleCondition.notify_one();
}

void Rack::addMainline(std::string mainline) {
	Plug plug(nullptr);
	plug.name = mainline;
	plugArray.push_back(plug);
}

void Rack::addUnit(std::unique_ptr<RackUnit> unit) {
	auto name = unit->getName();
	if(unit->getRackQueue().expired()) {
		unit->setRackQueue(mRackQueue);
	}
	unit->setEventLoop(&eventLoop);
	mUnits.insert(std::pair<std::string, RackUnitShr>(
				name, RackUnitShr(unit.release())
				));
}

bool Rack::hasUnit(std::string label) {
	if(mUnits.find(label) == mUnits.end())
		return false;

	return true;
}

void Rack::connectUnits(std::string from, std::string plug, std::string to, std::string jack) {
	if(from == "rack") {
		auto mainline = getPlug(plug);
		auto unit = getUnit(to);
		mainline->connected = true;
		mainline->jack = unit->getJack(jack);
		mainline->jack->connected = true;
		mainline->unit = unit.get();
	} else {
		auto uTo = getUnit(to);
		auto uFrom = getUnit(from);

		uFrom->setConnection(plug, jack, uTo.get());
	}
}

MidiHandler& Rack::getMidiHandler() {
	return midiHandler;
}

void Rack::setRackQueue(std::unique_ptr<RackQueue> queue) {
	mRackQueue = std::shared_ptr<RackQueue>(queue.release());
}

std::shared_ptr<RackQueue> Rack::getRackQueue() {
	return mRackQueue;
}

// Telemetry
#if RACK_METRICS
void Rack::cbmetricUnitCycle(
		std::function<void(std::chrono::steady_clock::time_point)> start,
		std::function<void(std::chrono::steady_clock::time_point)> end
) {
	metricUnitCycleStart = start;
	metricUnitCycleEnd = end;
}
#endif
