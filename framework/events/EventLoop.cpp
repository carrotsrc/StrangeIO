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

using namespace strangeio;



EventLoop::EventLoop() {
}

void EventLoop::initEvents(short numEvents) {
	while(numEvents-- > 0)
		eventListeners.insert(
				std::make_pair(
					numEvents,
					std::vector< std::function< void(std::shared_ptr<EventMessage>) > >())
					);
}

void EventLoop::addEventListener(EventType event, std::function<void(std::shared_ptr<EventMessage>)> callback) {
	eventListeners[event].push_back(callback);
}

void EventLoop::addEvent(std::unique_ptr<EventMessage> msg) {
	std::unique_lock<std::mutex> mlock(evLock);
		eventQueue.push_back(std::move(msg));
		mData = true;
	mlock.unlock();
	this->cv.notify_one();
}

void EventLoop::cycle() {
	mData = false;
	std::vector< std::unique_ptr<EventMessage> >::iterator qit;
	std::unique_ptr<EventMessage> ptr;
	std::unique_lock<std::mutex> mlock(evLock, std::defer_lock);
	mActive = true;
	mRunning = true;
	while(mRunning) {
		mlock.lock();
		cv.wait(mlock, [this]{ return this->unblock(); });
			if(!mRunning) {
				eventQueue.clear();
				mlock.unlock();
				break;
			}

			ptr = nullptr;
			if(eventQueue.size()) {
				qit = eventQueue.begin();
				ptr = std::move(*qit);
				eventQueue.erase(qit);

				if(!eventQueue.size())
					mData = false;
			}
			else {
				mData = false;
			}
		mlock.unlock();

		if(ptr != nullptr)
			distributeMessage(std::move(ptr));
	}
	mActive = false;
}



void EventLoop::distributeMessage(std::unique_ptr<EventMessage> msg) {
	std::vector< std::function< void(std::shared_ptr<EventMessage>) > >::iterator it;
	std::shared_ptr<EventMessage> sharedMsg(std::move(msg));
	EventType type = sharedMsg->msgType;
	for(it = eventListeners[type].begin();
	    it != eventListeners[type].end();
	    ++it) {
		(*it)(sharedMsg);
	}
}

void EventLoop::start() {
	mActive = false;
	mRunning = false;
	mLoopThread = std::thread(&EventLoop::cycle, this);
	while(!mRunning)
		continue;
}

void EventLoop::stop() {
	mRunning = false;
	cv.notify_one();
	mLoopThread.join();
}

bool EventLoop::unblock() {
	if(mData || !mRunning)
		return true;

	return false;
}

bool EventLoop::isRunning() {
	return mRunning;
}

bool EventLoop::isActive() {
	return mActive;
}

void EventLoop::frameworkInit() {

}
