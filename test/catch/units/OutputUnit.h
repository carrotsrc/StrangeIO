/* Copyright (C) 2015  Charlie Fyvie-Gauld
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef __OUTPUTUNIT_H_1439299905__
#define __OUTPUTUNIT_H_1439299905__
#include "framework/rack/RackUnit.h"
#include "TestingUnit.h"
 
class OutputUnit : public StrangeIO::Testing::TestingUnit
{
public:
	enum WorkState {
		IDLE,
		INIT,
		READY
	};

	OutputUnit();
	StrangeIO::FeedState feed(StrangeIO::Jack*);
	void setConfig(std::string, std::string);
	StrangeIO::RackState init();
	StrangeIO::RackState cycle();
	void block(StrangeIO::Jack*);
private:
	WorkState workState;

	void task();
};
#endif

