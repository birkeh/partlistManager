#ifndef CWIDGET_H
#define CWIDGET_H


#include "cmanufacturerwindow.h"
#include "cdistributorwindow.h"

#include <QWidget>
#include <QMdiSubWindow>


/*!
 \brief

 \class cWidget cwidget.h "cwidget.h"
*/
class cWidget : public QWidget
{
	Q_OBJECT
public:
	/*!
	 \brief

	 \enum TYPE
	*/
	enum TYPE
	{
		TYPE_unknown		= 0,
		TYPE_manufacturer	= 1,
		TYPE_distributor	= 2,
		TYPE_storage		= 3,
		TYPE_part			= 4,
		TYPE_project		= 5,
	};

	explicit			cWidget(cManufacturerWindow* parent);
	explicit			cWidget(cDistributorWindow* parent);
	explicit			cWidget(QWidget* parent);

	QWidget*			widget();

	void				setWindow(QMdiSubWindow* lpWindow);
	QMdiSubWindow*		window();
	TYPE				type();
signals:

public slots:

private:
	TYPE				m_type;
	QWidget*			m_lpWidget;
	QMdiSubWindow*		m_lpWindow;
};

#endif // CWIDGET_H
