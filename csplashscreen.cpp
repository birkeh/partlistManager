/*!
 \file csplashscreen.cpp

*/

#include "csplashscreen.h"
//#include "common.h"

#include <QStyleOptionProgressBar>


cSplashScreen::cSplashScreen(const QPixmap& pixmap, QFont& font) :
	QSplashScreen(pixmap),
	m_iMax(100),
	m_iProgress(0)
{
	setFont(font);
	m_textDocument.setDefaultFont(font);
}

void cSplashScreen::setMax(qint32 max)
{
	m_iMax	= max;
}

void cSplashScreen::drawContents(QPainter *painter)
{
	painter->translate(m_rect.topLeft());
	m_textDocument.setHtml(m_szMessage);
	m_textDocument.drawContents(painter);

	QStyleOptionProgressBar	pbstyle;
	pbstyle.initFrom(this);
	pbstyle.state = QStyle::State_Enabled;
	pbstyle.textVisible = false;
	pbstyle.minimum = 0;
	pbstyle.maximum = m_iMax;
	pbstyle.progress = m_iProgress;
	pbstyle.invertedAppearance = false;
	pbstyle.rect = QRect(0, 330, 390, 10); // Where is it.

	// Draw it...
	style()->drawControl(QStyle::CE_ProgressBar, &pbstyle, painter, this);
}

void cSplashScreen::showStatusMessage(const QString& message)
{
	m_szMessage	= message;
	showMessage(m_szMessage);
}

void cSplashScreen::addStatusMessage(const QString& message)
{
	m_szMessage.append(message);
	showMessage(m_szMessage);
}

void cSplashScreen::setMessageRect(QRect rect)
{
	m_rect			= rect;
	m_textDocument.setTextWidth(rect.width());
}

void cSplashScreen::setProgress(int value)
{
	m_iProgress		= value;
	update();
}
