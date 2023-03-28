/*!
 \file csplashscreen.h

*/

#ifndef CSPLASHSCREEN_H
#define CSPLASHSCREEN_H


#include <QSplashScreen>
#include <QPainter>
#include <QTextDocument>


/*!
 \brief

 \class cSplashScreen csplashscreen.h "csplashscreen.h"
*/
class cSplashScreen : public QSplashScreen
{
public:
	cSplashScreen(const QPixmap& pixmap, QFont &font);

	/*!
	 \brief

	 \fn drawContents
	 \param painter
	*/
	virtual void	drawContents(QPainter *painter);
	/*!
	 \brief

	 \fn showStatusMessage
	 \param message
	*/
	void			showStatusMessage(const QString &message);
	/*!
	 \brief

	 \fn addStatusMessage
	 \param message
	*/
	void			addStatusMessage(const QString &message);
	/*!
	 \brief

	 \fn setMessageRect
	 \param rect
	*/
	void			setMessageRect(QRect rect);

	/*!
	 \brief

	 \fn setMax
	 \param max
	*/
	void			setMax(qint32 max);

private:
	QTextDocument	m_textDocument;					/*!< TODO: describe */
	QString			m_szMessage;					/*!< TODO: describe */
	QRect			m_rect;							/*!< TODO: describe */
	qint32			m_iMax;							/*!< TODO: describe */
	qint32			m_iProgress;					/*!< TODO: describe */
public slots:
	/*!
	 \brief

	 \fn setProgress
	 \param value
	*/
	void			setProgress(int value);
};

#endif // CSPLASHSCREEN_H
