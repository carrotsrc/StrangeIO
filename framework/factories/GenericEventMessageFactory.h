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
#ifndef GENERICEVENTMESSAGEFACTORY_H
#define GENERICEVENTMESSAGEFACTORY_H

#include "common.h"
#include "framework/events/EventMessage.h"

namespace RackoonIO {

class GenericEventMessageFactory {
public:
	virtual std::unique_ptr<EventMessage> createMessage(EventType) = 0;
};

}
#endif
