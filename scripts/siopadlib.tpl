@HEADER@
#ifndef @DEF@_HPP__
#define @DEF@_HPP__
#include <QWidget>

extern "C" QWidget *@UNIT@Build(const QString & panelId);

#endif
@SOURCE@
#include "@UNIT@.hpp"

extern "C" QWidget *@UNIT@Build(const QString & panelId) {
/*
	if(panelId == "SpUnit") {
		auto obj = new SpUnit();
		return obj;
	}
*/

	return nullptr;
}

