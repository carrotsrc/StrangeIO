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

void EventLoop::initEvents(int numEvents) {
	while(numEvents-- > 0)
		eventListeners.push_back(std::vector< std::function< void(shared_ptr<EventMessage>) > >());
}

void EventLoop::addEventListener(EventType event, std::function<void(shared_ptr<EventMessage>)> callback) {
	eventListeners[event].push_back(callback);
}

void EventLoop::addEvent(unique_ptr<EventMessage> msg) {
	if(msg->msgType >= eventListeners.size())
		return;

	evLock.lock();
	eventQueue.push_back(std::move(msg));
	evLock.unlock();
}

void EventLoop::cycle() {
	if(!evLock.try_lock())
		return;

	std::vector< std::unique_ptr<EventMessage> >::iterator qit;
	int i = 0;
	for(qit = eventQueue.begin();qit != eventQueue.end();++qit) {
		distributeMessage(std::move(*qit));
		qit = eventQueue.erase(qit);
		if(qit == eventQueue.end())
			break;
	}

	evLock.unlock();
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
