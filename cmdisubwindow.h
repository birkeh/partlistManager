/*!
 \file cmdisubwindow.h

*/

#ifndef CMDISUBWINDOW_H
#define CMDISUBWINDOW_H


#include <QWidget>


/*!
 \brief

 \class cMDISubWindow cmdisubwindow.h "cmdisubwindow.h"
*/
class cMDISubWindow : public QWidget
{
	Q_OBJECT
public:
	/*!
	 \brief

	 \fn cMDISubWindow
	 \param parent
	*/
	explicit cMDISubWindow(QWidget *parent = nullptr);

signals:
	/*!
	 \brief

	 \fn subWindowClosed
	 \param lpWidget
	*/
	void					subWindowClosed(QWidget* lpWidget);

public slots:

protected:
	/*!
	 \brief

	 \fn closeEvent
	 \param event
	*/
	void					closeEvent(QCloseEvent *event);
};

#endif // CMDISUBWINDOW_H
