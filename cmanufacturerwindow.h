#ifndef CMANUFACTURERWINDOW_H
#define CMANUFACTURERWINDOW_H


#include "cmanufacturer.h"

#include "cmdisubwindow.h"
#include "cmainwindow.h"

#include <QWidget>


namespace Ui {
class cManufacturerWindow;
}

class cManufacturerWindow : public cMDISubWindow
{
	Q_OBJECT

public:
	explicit cManufacturerWindow(QWidget *parent = nullptr);
	~cManufacturerWindow();

	void						setManufacturer(cManufacturer* manufacturer);
	cManufacturer*				manufacturer();

private:
	Ui::cManufacturerWindow*	ui;
	cManufacturer*				m_manufacturer;

	void						fillFields();

public slots:
	void						onManufacturerChanged(cManufacturer* manufacturer);

private slots:
	void						onNameChanged(const QString& szName);
	void						onAddressChanged();
	void						onURLChanged(const QString& szURL);
	void						onEmailChanged(const QString& szEmail);
	void						onPhoneChanged(const QString& szPhone);
	void						onFaxChanged(const QString& szFax);
	void						onCommentChanged();

signals:
	void						somethingChanged();
};

#endif // CMANUFACTURERWINDOW_H
