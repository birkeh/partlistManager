#ifndef CMAINWINDOW_H
#define CMAINWINDOW_H


#include "csplashscreen.h"

#include "cdatabase.h"
#include "cmanufacturer.h"
#include "cdistributor.h"
#include "cstoragecategory.h"
#include "cstorage.h"

#include <QMainWindow>
#include <QCloseEvent>
#include <QStandardItemModel>
#include <QMdiSubWindow>


QT_BEGIN_NAMESPACE
namespace Ui { class cMainWindow; }
QT_END_NAMESPACE

class cMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	cMainWindow(cSplashScreen* splashScreen, QWidget *parent = nullptr);
	~cMainWindow();

private:
	Ui::cMainWindow*				ui;

	bool							m_somethingChanged;

	cSplashScreen*					m_splashScreen;
	cDatabase*						m_database;
	cManufacturerList*				m_manufacturerList;
	cDistributorList*				m_distributorList;
	cStorageCategoryList*			m_storageCategoryList;
	cStorageList*					m_storageList;

	QMenu*							m_fileMenu;

	QToolBar*						m_fileToolBar;

	QAction*						m_fileSaveAction;
	QAction*						m_fileQuitAction;

	QAction*						m_listAdd;
	QAction*						m_listEdit;
	QAction*						m_listRemove;

	QAction*						m_manufacturerListAddAction;
	QAction*						m_manufacturerListEditAction;
	QAction*						m_manufacturerListRemoveAction;

	QAction*						m_distributorListAddAction;
	QAction*						m_distributorListEditAction;
	QAction*						m_distributorListRemoveAction;

	QStandardItemModel*				m_manufacturerListModel;
	QStandardItemModel*				m_distributorListModel;
	QStandardItemModel*				m_storageListModel;

	bool							m_updatingTab;


	void							initUI();
	void							createActions();
	void							createFileActions();
	void							createContextActions();

	void							loadData();

	void							setListButtonState();

	qint16							activeTab();

	void							updateWindowTitle();

	void							addManufacturer();
	void							editManufacturer();
	void							removeManufacturer();

	void							addDistributor();
	void							editDistributor();
	void							removeDistributor();

protected:
	void							closeEvent(QCloseEvent* event);

private slots:
	void							onSomethingChanged();

	void							onToolBoxCurrentChanged(int index);

	void							onFileSave();

	void							onMainTabCurrentChanged(int index);
	void							onMainTabTabCloseRequested(int index);
	void							onMdiAreaSubWindowActivated(QMdiSubWindow *arg1);
	void							onSubWindowClosed(QWidget* lpSubWindow);

	void							onManufacturerListClicked(const QModelIndex& index);
	void							onManufacturerListDoubleClicked(const QModelIndex& index);
	void							onManufacturerListContextMenu(const QPoint& pos);
	void							onManufacturerListNameChanged(QStandardItem* item);
	void							onManufacturerListAddClicked();
	void							onManufacturerListEditClicked();
	void							onManufacturerListRemoveClicked();

	void							onDistributorListClicked(const QModelIndex& index);
	void							onDistributorListDoubleClicked(const QModelIndex& index);
	void							onDistributorListContextMenu(const QPoint& pos);
	void							onDistributorListNameChanged(QStandardItem* item);
	void							onDistributorListAddClicked();
	void							onDistributorListEditClicked();
	void							onDistributorListRemoveClicked();

	void							onListAdd();
	void							onListEdit();
	void							onListRemove();

signals:
	void							manufacturerNameChanged(cManufacturer* manufacturer);
	void							distributorNameChanged(cDistributor* distributor);
};

#endif // CMAINWINDOW_H
