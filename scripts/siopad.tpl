@HEADER@
#ifndef @DEF@_HPP__
#define @DEF@_HPP__
 
#include <QWidget>

#include "ui/SPad.hpp"
#include "@UNIT@.hpp"

class @UNIT@ : public SPad
{
Q_OBJECT
public:
	explicit @UNIT@(QWidget *parent = 0);
	
public slots:
	void onGuiUpdate();
	
signals:
	void guiUpdate();

public:
	QSize sizeHint() const;

private:
	void onRegisterUnit();
};

#endif
@SOURCE@
#include "@UNIT@.hpp"

@UNIT@::@UNIT@(QWidget* parent)
	: SPad(parent)
{
	
}

void @UNIT@::onRegisterUnit() {
	connect(this, SIGNAL(guiUpdate()), this, SLOT(onGuiUpdate()));
/*
	if(auto u = unit<RackUnit>()) {

	}
*/
}

void @UNIT@::onGuiUpdate() {
	
}

QSize @UNIT@::sizeHint() const {
	return QSize(width(), height());
}


