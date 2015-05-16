/*
 * Copyright (C) 2015  Charlie Fyvie-Gauld
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
#ifndef LV2ADAPTOR_H
#define LV2ADAPTOR_H
#include "framework/common.h"
#include "framework/rack/RackUnit.h"
#include "LV2Platform.h"

namespace RackoonIO {
namespace Hosting {
/**
 * A general purpose adaptor for 
 */
class LV2Adaptor : public RackUnit {
public:
	LV2Adaptor(std::string);
	void setPlatform(const LV2Platform*);

	virtual RackState init() = 0;
	virtual void setConfig(std::string, std::string) = 0;
	virtual FeedState feed(Jack*) = 0;
	virtual RackState cycle() = 0;
	virtual void block(Jack*) = 0;


protected:
	std::unique_ptr<LV2Plugin> mPlugin;

	void loadPlugin(std::string, bool = false);
	virtual void feedPlugin(Jack*) = 0;
	bool initPlugin();

private:

	const LV2Platform *mPlatform;
};

} // Hosting
} // RackoonIO
#endif
