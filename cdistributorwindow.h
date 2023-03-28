#ifndef CDISTRIBUTORWINDOW_H
#define CDISTRIBUTORWINDOW_H


#include "cdistributor.h"

#include "cmdisubwindow.h"
#include "cmainwindow.h"

#include <QWidget>


namespace Ui {
class cDistributorWindow;
}

class cDistributorWindow : public cMDISubWindow
{
	Q_OBJECT

public:
	explicit cDistributorWindow(QWidget *parent = nullptr);
	~cDistributorWindow();

	void						setDistributor(cDistributor* distributor);
	cDistributor*				distributor();

private:
	Ui::cDistributorWindow*	ui;
	cDistributor*				m_distributor;

	void						fillFields();

public slots:
	void						onDistributorChanged(cDistributor* distributor);

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

#endif // CDISTRIBUTORWINDOW_H
