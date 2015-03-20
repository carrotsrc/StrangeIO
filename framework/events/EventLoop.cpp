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
#include "EventLoop.h"

using namespace RackoonIO;



EventLoop::EventLoop() {
}

void EventLoop::initEvents(short numEvents) {
	while(numEvents-- > 0)
		eventListeners.insert(
				std::make_pair(
					numEvents, 
					std::vector< std::function< void(shared_ptr<EventMessage>) > >())
					);
}

void EventLoop::addEventListener(EventType event, std::function<void(shared_ptr<EventMessage>)> callback) {
	eventListeners[event].push_back(callback);
}

void EventLoop::addEvent(unique_ptr<EventMessage> msg) {
	evLock.lock();
	eventQueue.push_back(std::move(msg));
	evLock.unlock();
}

void EventLoop::cycle() {
	mRunning = true;
	mData = false;
	std::vector< std::unique_ptr<EventMessage> >::iterator qit;
	std::unique_ptr<EventMessage> ptr;

	while(mRunning) {
		evLock.lock();
			qit = eventQueue.begin();
			ptr = std::move(*qit); 
			if(eventQueue.erase(qit) == eventQueue.end())
				mData = false;
		evLock.unlock();
	}
	distributeMessage(std::move(ptr));
}

void EventLoop::distributeMessage(std::unique_ptr<EventMessage> msg) {
	std::vector< std::function< void(shared_ptr<EventMessage>) > >::iterator it;
	std::shared_ptr<EventMessage> sharedMsg(std::move(msg));
	EventType type = sharedMsg->msgType;
	for(it = eventListeners[type].begin();
	    it != eventListeners[type].end();
	    ++it) {
		(*it)(sharedMsg);
	}
}

void EventLoop::frameworkInit() {

}
