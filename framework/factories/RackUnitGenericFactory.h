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
#ifndef RACKUNITGENERICFACTORY_H
#define RACKUNITGENERICFACTORY_H
#include "framework/rack/RackUnit.h"
#include "framework/factories/GenericEventMessageFactory.h"
namespace RackoonIO {
class RackUnitGenericFactory {
protected:
	EventLoop *eventLoop;
	GenericEventMessageFactory *messageFactory;
	void setDependencies(RackUnit *unit);

public:
	virtual std::unique_ptr<RackUnit> build(std::string, std::string) = 0;
	void setEventLoop(EventLoop*);
	void setMessageFactory(GenericEventMessageFactory*);

};

}
#endif
