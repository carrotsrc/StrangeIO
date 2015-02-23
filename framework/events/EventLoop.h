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
#ifndef EVENTLOOP_H
#define EVENTLOOP_H
#include "common.h"
#include "framework/factories/GenericEventMessageFactory.h"

namespace RackoonIO {


class EventLoop {
	std::vector< std::vector< std::function< void(shared_ptr<EventMessage>) > > > eventListeners;
	std::vector< std::unique_ptr<EventMessage> > eventQueue;
	int maxEventTypes;

	std::mutex evLock;

	void distributeMessage(std::unique_ptr<EventMessage>);
public:
	EventLoop();

	void addEventListener(EventType, std::function<void(shared_ptr<EventMessage>)>);
	void addEvent(std::unique_ptr<EventMessage>);
	void cycle();

	void initEvents(int);
};
}
#endif
