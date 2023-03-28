/*!
 \file cmdisubwindow.cpp

*/

#include "cmdisubwindow.h"

#include <QCloseEvent>


cMDISubWindow::cMDISubWindow(QWidget *parent) : QWidget(parent)
{
}

void cMDISubWindow::closeEvent(QCloseEvent* event)
{
	emit subWindowClosed(this);
	event->accept();
}
