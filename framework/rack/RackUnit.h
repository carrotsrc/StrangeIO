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
#ifndef RACKUNIT_H
#define RACKUNIT_H
#include <functional>


#include "UnitConnectors.h"
#include "framework/threads/RackQueue.h"
#include "framework/events/EventLoop.h"
#include "framework/factories/GenericEventMessageFactory.h"

namespace RackoonIO {
#define MIDI_BIND(name, func) (midiExportMethod(name, std::bind(&func, this, std::placeholders::_1)))
#define EVENT_LISTENER(type, callback) (addEventListener(type, std::bind(&callback, this, std::placeholders::_1)))
#define OUTSRC(func) (outsource(std::bind(&func, this)))
#define CONSOLE_MSG(name, msg) (cout << name << " [" << getName() << "]: " << msg << endl)
class RackChain;
enum UnitState {
	UNIT_ACTIVE,
	UNIT_OFF
};

class RackUnit
{
	vector<Jack*> jackArray;
	vector<Plug*> plugArray;

	std::string name;
	std::string rutype;

	RackQueue *rackQueue;
	GenericEventMessageFactory *messageFactory;
	EventLoop *eventLoop;

	std::map<string, std::function<void(int)> > midiExport;

protected:
	RackChain *chain;
	UnitState unitState;

	void addJack(string, ConnectorType);
	void addPlug(string);

	void feedOut(string, short*);

	void outsource(std::function<void()>);
	void midiExportMethod(string, std::function<void(int)> );

	void addEventListener(EventType, std::function<void(shared_ptr<EventMessage>)>);
	void addEvent(std::unique_ptr<EventMessage>);
	unique_ptr<EventMessage> createMessage(EventType);
public:
	RackUnit(string);

	void setName(string);
	string getName() const;
	std::string getRuType() const;

	void setChain(RackChain*);
	RackChain *getChain() const;

	void setRackQueue(RackQueue*);
	void setMessageFactory(GenericEventMessageFactory*);

	void setEventLoop(EventLoop*);

	Jack *getJack (string) const;
	Plug *getPlug (string) const;

	void setConnection(string, string, RackUnit*);

	void join();
	void unjoin();

	RackState rackFeed(RackState);

	bool midiControllable();
	virtual std::map<string, std::function<void(int)> > midiExportedMethods();

	virtual void setConfig(string, string) = 0;
	virtual FeedState feed(Jack*) = 0;

	virtual RackState cycle() = 0;
	virtual RackState init() = 0;

	virtual void block(Jack*) = 0;

};

}
#endif
