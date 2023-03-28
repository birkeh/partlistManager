#include "cmainwindow.h"
#include "ui_cmainwindow.h"

#include "cdatabase.h"

#include "cwidget.h"
#include "cmanufacturerwindow.h"
#include "cdistributorwindow.h"

#include <QSettings>
#include <QInputDialog>

#include <QMessageBox>

#include "common.h"


cMainWindow::cMainWindow(cSplashScreen* splashScreen, QWidget *parent)
	: QMainWindow(parent),
	  ui(new Ui::cMainWindow),
	  m_somethingChanged(false),
	  m_splashScreen(splashScreen),
	  m_database(nullptr),
	  m_manufacturerList(nullptr),
	  m_distributorList(nullptr),
	  m_storageCategoryList(nullptr),
	  m_storageList(nullptr),
	  m_fileMenu(nullptr),
	  m_fileToolBar(nullptr),
	  m_fileQuitAction(nullptr),
	  m_listAdd(nullptr),
	  m_listEdit(nullptr),
	  m_listRemove(nullptr),
	  m_manufacturerListAddAction(nullptr),
	  m_manufacturerListEditAction(nullptr),
	  m_manufacturerListRemoveAction(nullptr),
	  m_distributorListAddAction(nullptr),
	  m_distributorListEditAction(nullptr),
	  m_distributorListRemoveAction(nullptr),
	  m_manufacturerListModel(nullptr),
	  m_distributorListModel(nullptr),
	  m_storageListModel(nullptr),
	  m_updatingTab(false)
{
	initUI();
	createActions();

	loadData();

	setListButtonState();
	updateWindowTitle();
}

cMainWindow::~cMainWindow()
{
	if(m_manufacturerList)
		delete m_manufacturerList;

	if(m_distributorList)
		delete m_distributorList;

	if(m_storageCategoryList)
		delete m_storageCategoryList;

	if(m_storageList)
		delete m_storageList;

	if(m_database)
		delete m_database;

	delete ui;
}

void cMainWindow::closeEvent(QCloseEvent *event)
{
	QSettings	settings;
	settings.setValue("main/width", QVariant::fromValue(size().width()));
	settings.setValue("main/height", QVariant::fromValue(size().height()));
	settings.setValue("main/x", QVariant::fromValue(x()));
	settings.setValue("main/y", QVariant::fromValue(y()));
	if(this->isMaximized())
		settings.setValue("main/maximized", QVariant::fromValue(true));
	else
		settings.setValue("main/maximized", QVariant::fromValue(false));

	QList<qint32>	sizes	= ui->m_splitter->sizes();

	for(int x = 0;x < sizes.count();x++)
		settings.setValue(QString("main/splitter%1").arg(x+1), QVariant::fromValue(sizes[x]));

	event->accept();
}

void cMainWindow::initUI()
{
	ui->setupUi(this);

	QIcon::setThemeName("TangoMFK");

	QSettings	settings;

	if(!settings.value("main/maximized").toBool())
	{
		qint32		iX		= settings.value("main/x", QVariant::fromValue(-1)).toInt();
		qint32		iY		= settings.value("main/y", QVariant::fromValue(-1)).toInt();
		qint32		iWidth	= settings.value("main/width", QVariant::fromValue(-1)).toInt();
		qint32		iHeight	= settings.value("main/height", QVariant::fromValue(-1)).toInt();

		if(iWidth != -1 && iHeight != -1)
			resize(iWidth, iHeight);
		if(iX != -1 && iY != -1)
			move(iX, iY);
	}

	qint32		iWidth1	= settings.value("main/splitter1", QVariant::fromValue(-1)).toInt();
	qint32		iWidth2	= settings.value("main/splitter2", QVariant::fromValue(-1)).toInt();
	qint32		iWidth3	= settings.value("main/splitter3", QVariant::fromValue(-1)).toInt();

	ui->m_splitter->setSizes(QList<int>() << iWidth1 << iWidth2 << iWidth3);

	m_manufacturerListModel		= new QStandardItemModel(0, 1);
	ui->m_manufacturerList->setModel(m_manufacturerListModel);

	m_distributorListModel		= new QStandardItemModel(0, 1);
	ui->m_distributorList->setModel(m_distributorListModel);

	m_storageListModel			= new QStandardItemModel(0, 1);
	ui->m_storageList->setModel(m_storageListModel);

	ui->m_toolBox->setCurrentIndex(0);
}

void cMainWindow::createActions()
{
	setToolButtonStyle(Qt::ToolButtonFollowStyle);
	createFileActions();

	const QIcon	addIcon			= QIcon::fromTheme("list-add");
	m_listAdd	= new QAction(addIcon, "add", this);
	ui->m_listAdd->setDefaultAction(m_listAdd);
	connect(m_listAdd,					&QAction::triggered,					this,	&cMainWindow::onListAdd);

	const QIcon	editIcon		= QIcon::fromTheme("accessories-text-editor");
	m_listEdit	= new QAction(editIcon, "edit", this);
	ui->m_listEdit->setDefaultAction(m_listEdit);
	connect(m_listEdit,					&QAction::triggered,					this,	&cMainWindow::onListEdit);

	const QIcon	removeIcon		= QIcon::fromTheme("list-remove");
	m_listRemove	= new QAction(removeIcon, "remove", this);
	ui->m_listRemove->setDefaultAction(m_listRemove);
	connect(m_listRemove,				&QAction::triggered,					this,	&cMainWindow::onListRemove);

	connect(ui->m_toolBox,				&QToolBox::currentChanged,				this,	&cMainWindow::onToolBoxCurrentChanged);

	connect(ui->m_manufacturerList,		&QTreeView::clicked,					this,	&cMainWindow::onManufacturerListClicked);
	connect(ui->m_manufacturerList,		&QTreeView::doubleClicked,				this,	&cMainWindow::onManufacturerListDoubleClicked);
	connect(m_manufacturerListModel,	&QStandardItemModel::itemChanged,		this,	&cMainWindow::onManufacturerListNameChanged);
	connect(ui->m_manufacturerList,		&QTreeView::customContextMenuRequested,	this,	&cMainWindow::onManufacturerListContextMenu);

	connect(ui->m_distributorList,		&QTreeView::clicked,					this,	&cMainWindow::onDistributorListClicked);
	connect(ui->m_distributorList,		&QTreeView::doubleClicked,				this,	&cMainWindow::onDistributorListDoubleClicked);
	connect(m_distributorListModel,		&QStandardItemModel::itemChanged,		this,	&cMainWindow::onDistributorListNameChanged);
	connect(ui->m_distributorList,		&QTreeView::customContextMenuRequested,	this,	&cMainWindow::onDistributorListContextMenu);

	connect(ui->m_mainTab,				&QTabWidget::currentChanged,			this,	&cMainWindow::onMainTabCurrentChanged);
	connect(ui->m_mainTab,				&QTabWidget::tabCloseRequested,			this,	&cMainWindow::onMainTabTabCloseRequested);
	connect(ui->m_mdiArea,				&QMdiArea::subWindowActivated,			this,	&cMainWindow::onMdiAreaSubWindowActivated);

	createContextActions();
}

void cMainWindow::createContextActions()
{
	m_manufacturerListAddAction	= new QAction(tr("add..."), this);
	connect(m_manufacturerListAddAction, &QAction::triggered, this, &cMainWindow::onManufacturerListAddClicked);

	m_manufacturerListEditAction	= new QAction(tr("edit..."), this);
	connect(m_manufacturerListEditAction, &QAction::triggered, this, &cMainWindow::onManufacturerListEditClicked);

	m_manufacturerListRemoveAction	= new QAction(tr("remove..."), this);
	connect(m_manufacturerListRemoveAction, &QAction::triggered, this, &cMainWindow::onManufacturerListRemoveClicked);

	m_distributorListAddAction	= new QAction(tr("add..."), this);
	connect(m_distributorListAddAction, &QAction::triggered, this, &cMainWindow::onDistributorListAddClicked);

	m_distributorListEditAction	= new QAction(tr("edit..."), this);
	connect(m_distributorListEditAction, &QAction::triggered, this, &cMainWindow::onDistributorListEditClicked);

	m_distributorListRemoveAction	= new QAction(tr("remove..."), this);
	connect(m_distributorListRemoveAction, &QAction::triggered, this, &cMainWindow::onDistributorListRemoveClicked);
}

void cMainWindow::createFileActions()
{
	m_fileMenu		= menuBar()->addMenu(tr("&File"));
	m_fileToolBar	= addToolBar(tr("File Actions"));

	m_fileSaveAction			= m_fileMenu->addAction(tr("&Save"), this, &cMainWindow::onFileSave);
	m_fileSaveAction->setShortcut(Qt::CTRL | Qt::Key_S);

	m_fileMenu->addSeparator();

	m_fileQuitAction			= m_fileMenu->addAction(tr("&Quit"), this, &QWidget::close);
	m_fileQuitAction->setShortcut(Qt::CTRL | Qt::Key_Q);
}

void cMainWindow::loadData()
{
	m_database	= new cDatabase();
	m_database->connect();

	m_manufacturerList	= new cManufacturerList(m_database);
	m_manufacturerList->load();
	m_manufacturerList->fillList(m_manufacturerListModel);

	m_distributorList	= new cDistributorList(m_database);
	m_distributorList->load();
	m_distributorList->fillList(m_distributorListModel);

	m_storageCategoryList	= new cStorageCategoryList(m_database);
	m_storageCategoryList->load();
	m_storageCategoryList->fillList(m_storageListModel);

	m_storageList	= new cStorageList(m_database, m_storageCategoryList);
	m_storageList->load();
	m_storageList->fillList();
}

void cMainWindow::setListButtonState()
{
	switch(activeTab())
	{
	case TAB_MANUFACTURER_LIST:
		if(ui->m_manufacturerList->selectionModel()->selectedRows().count())
		{
			m_listEdit->setEnabled(true);
			m_listRemove->setEnabled(true);
		}
		else
		{
			m_listEdit->setEnabled(false);
			m_listRemove->setEnabled(false);
		}
		break;
	case TAB_DISTRIBUTOR_LIST:
		if(ui->m_distributorList->selectionModel()->selectedRows().count())
		{
			m_listEdit->setEnabled(true);
			m_listRemove->setEnabled(true);
		}
		else
		{
			m_listEdit->setEnabled(false);
			m_listRemove->setEnabled(false);
		}
		break;
	case TAB_PART_LIST:
		{
			m_listEdit->setEnabled(false);
			m_listRemove->setEnabled(false);
		}
		break;
	case TAB_PROJECT_LIST:
		{
			m_listEdit->setEnabled(false);
			m_listRemove->setEnabled(false);
		}
		break;
	}

	m_listAdd->setEnabled(true);
}

void cMainWindow::onFileSave()
{
	if(m_manufacturerList->save())
	{
		m_somethingChanged	= false;
		updateWindowTitle();
	}

	if(m_distributorList->save())
	{
		m_somethingChanged	= false;
		updateWindowTitle();
	}

	if(m_storageList->save())
	{
		m_somethingChanged	= false;
		updateWindowTitle();
	}

	if(m_storageCategoryList->save())
	{
		m_somethingChanged	= false;
		updateWindowTitle();
	}
}

void cMainWindow::onSomethingChanged()
{
	if(!m_somethingChanged)
	{
		m_somethingChanged	= true;
		updateWindowTitle();
	}
}

void cMainWindow::onToolBoxCurrentChanged(int /*index*/)
{
	setListButtonState();
}

void cMainWindow::onMainTabCurrentChanged(int /*index*/)
{
	if(m_updatingTab)
		return;

	m_updatingTab	= true;
	cWidget*		lpWidget	= static_cast<cWidget*>(ui->m_mainTab->currentWidget());
	QMdiSubWindow*	lpWindow	= lpWidget->window();
	ui->m_mdiArea->setActiveSubWindow(lpWindow);
	m_updatingTab	= false;
}

void cMainWindow::onMainTabTabCloseRequested(int index)
{
	if(m_updatingTab)
		return;

	m_updatingTab	= true;
//	disconnectTextEdit();
//	m_lpOldTextEdit	= nullptr;
	cWidget*		lpWidget	= static_cast<cWidget*>(ui->m_mainTab->currentWidget());
	QMdiSubWindow*	lpWindow	= lpWidget->window();
	ui->m_mainTab->removeTab(index);
	ui->m_mdiArea->removeSubWindow(lpWindow);
	delete(lpWidget);
	m_updatingTab	= false;
}

void cMainWindow::onMdiAreaSubWindowActivated(QMdiSubWindow *arg1)
{
	if(m_updatingTab)
		return;

	m_updatingTab	= true;

	for(int x = 0;x < ui->m_mainTab->count();x++)
	{
		cWidget*	lpWidget	= static_cast<cWidget*>(ui->m_mainTab->widget(x));
		if(lpWidget->window() == arg1)
		{
			ui->m_mainTab->setCurrentIndex(x);
			m_updatingTab	= false;
			return;
		}
	}
	m_updatingTab	= false;
}

void cMainWindow::onSubWindowClosed(QWidget* lpSubWindow)
{
	if(m_updatingTab)
		return;

	m_updatingTab	= true;
//	disconnectTextEdit();
//	m_lpOldTextEdit	= nullptr;

	for(int x = 0;x < ui->m_mainTab->count();x++)
	{
		cWidget*	lpWidget	= static_cast<cWidget*>(ui->m_mainTab->widget(x));
		if(lpWidget->widget() == lpSubWindow)
		{
			ui->m_mainTab->removeTab(x);
			m_updatingTab	= false;
			return;
		}
	}
	m_updatingTab	= false;
}

void cMainWindow::onManufacturerListClicked(const QModelIndex& /*index*/)
{
	setListButtonState();
}

void cMainWindow::onManufacturerListDoubleClicked(const QModelIndex& /*index*/)
{
	editManufacturer();
}

void cMainWindow::onManufacturerListContextMenu(const QPoint& pos)
{
	QMenu*			menu	= new QMenu(this);

	menu->addAction(m_manufacturerListAddAction);

	QStandardItem*	item	= nullptr;

	if(ui->m_manufacturerList->selectionModel()->selectedRows().count())
		item	= m_manufacturerListModel->itemFromIndex(ui->m_manufacturerList->currentIndex());
	if(item)
	{
		menu->addAction(m_manufacturerListEditAction);
		menu->addAction(m_manufacturerListRemoveAction);
	}

	menu->exec(ui->m_manufacturerList->mapToGlobal(pos));
}

void cMainWindow::onManufacturerListNameChanged(QStandardItem* item)
{
	cManufacturer*	manufacturer	= item->data(ITEM_MANUFACTURER_DATA).value<cManufacturer*>();
	QString			name			= item->text();

	if(name.isEmpty())
	{
		item->setText(manufacturer->name());
		return;
	}

	if(name != manufacturer->name())
	{
		onSomethingChanged();
		manufacturer->setName(name);
		emit manufacturerNameChanged(manufacturer);
	}
}

void cMainWindow::onDistributorListClicked(const QModelIndex& /*index*/)
{
	setListButtonState();
}

void cMainWindow::onDistributorListDoubleClicked(const QModelIndex& /*index*/)
{
	editDistributor();
}

void cMainWindow::onDistributorListContextMenu(const QPoint& pos)
{
	QMenu*			menu	= new QMenu(this);

	menu->addAction(m_distributorListAddAction);

	QStandardItem*	item	= nullptr;

	if(ui->m_distributorList->selectionModel()->selectedRows().count())
		item	= m_distributorListModel->itemFromIndex(ui->m_distributorList->currentIndex());
	if(item)
	{
		menu->addAction(m_distributorListEditAction);
		menu->addAction(m_distributorListRemoveAction);
	}

	menu->exec(ui->m_distributorList->mapToGlobal(pos));
}

void cMainWindow::onDistributorListNameChanged(QStandardItem* item)
{
	cDistributor*	distributor	= item->data(ITEM_DISTRIBUTOR_DATA).value<cDistributor*>();
	QString			name			= item->text();

	if(name.isEmpty())
	{
		item->setText(distributor->name());
		return;
	}

	if(name != distributor->name())
	{
		onSomethingChanged();
		distributor->setName(name);
		emit distributorNameChanged(distributor);
	}
}

void cMainWindow::onListAdd()
{
	switch(activeTab())
	{
	case TAB_MANUFACTURER_LIST:
		addManufacturer();
		break;
	case TAB_DISTRIBUTOR_LIST:
		addDistributor();
		break;
	}
}

void cMainWindow::onListEdit()
{
	switch(activeTab())
	{
	case TAB_MANUFACTURER_LIST:
		editManufacturer();
		break;
	}
	switch(activeTab())
	{
	case TAB_DISTRIBUTOR_LIST:
		editDistributor();
		break;
	}
}

void cMainWindow::onListRemove()
{
	switch(activeTab())
	{
	case TAB_MANUFACTURER_LIST:
		removeManufacturer();
		break;
	}
	switch(activeTab())
	{
	case TAB_DISTRIBUTOR_LIST:
		removeDistributor();
		break;
	}
}

void cMainWindow::onManufacturerListAddClicked()
{
	addManufacturer();
}

void cMainWindow::onManufacturerListEditClicked()
{
	editManufacturer();
}

void cMainWindow::onManufacturerListRemoveClicked()
{
	removeManufacturer();
}

void cMainWindow::onDistributorListAddClicked()
{
	addDistributor();
}

void cMainWindow::onDistributorListEditClicked()
{
	editDistributor();
}

void cMainWindow::onDistributorListRemoveClicked()
{
	removeDistributor();
}

qint16 cMainWindow::activeTab()
{
	QWidget*	widget	= ui->m_toolBox->currentWidget();

	if(widget == ui->m_manufacturerListTab)
		return(TAB_MANUFACTURER_LIST);
	else if(widget == ui->m_distributorListTab)
		return(TAB_DISTRIBUTOR_LIST);
	else if(widget == ui->m_partListTab)
		return(TAB_PART_LIST);
	else if(widget == ui->m_projectListTab)
		return(TAB_PROJECT_LIST);

	return(0);
}

void cMainWindow::updateWindowTitle()
{
	QString	windowTitle	= "";

	if(m_somethingChanged)
		windowTitle	= "unsaved";

	m_fileSaveAction->setEnabled(m_somethingChanged);

	setWindowTitle(windowTitle);
}

void cMainWindow::addManufacturer()
{
	bool	ok;
	QString			name			= QInputDialog::getText(this, tr("New Manufacturer"), tr("Name:"), QLineEdit::Normal, "", &ok);
	if(!ok)
		return;

	cManufacturer*	manufacturer	= m_manufacturerList->add(name);

	QStandardItem*	item			= new QStandardItem(name);
	item->setData(QVariant::fromValue(manufacturer), ITEM_MANUFACTURER_DATA);
	m_manufacturerListModel->appendRow(item);
	m_manufacturerListModel->sort(Qt::AscendingOrder | Qt::CaseInsensitive);

	onSomethingChanged();
}

void cMainWindow::editManufacturer()
{
	if(!ui->m_manufacturerList->selectionModel()->selectedIndexes().count())
		return;

	QModelIndex				index				= ui->m_manufacturerList->selectionModel()->selectedIndexes()[0];
	QStandardItem*			item				= m_manufacturerListModel->itemFromIndex(index);
	cManufacturer*			manufacturer		= item->data(ITEM_MANUFACTURER_DATA).value<cManufacturer*>();

	for(int x = 0;x < ui->m_mainTab->count();x++)
	{
		cWidget*	widget	= static_cast<cWidget*>(ui->m_mainTab->widget(x));
		if(widget->type() == cWidget::TYPE_manufacturer)
		{
			cManufacturerWindow*	manufacturerWindow	= static_cast<cManufacturerWindow*>(widget->widget());
			if(manufacturerWindow->manufacturer() == manufacturer)
			{
				ui->m_mainTab->setCurrentIndex(x);
				ui->m_mdiArea->setActiveSubWindow(widget->window());
				m_updatingTab	= false;
				return;
			}
		}
	}

	cManufacturerWindow*	manufacturerWindow	= new cManufacturerWindow(this);
	manufacturerWindow->setManufacturer(manufacturer);
	cWidget*				widget1				= new cWidget(manufacturerWindow);
	widget1->setWindow(ui->m_mdiArea->addSubWindow(manufacturerWindow));
	ui->m_mainTab->addTab(static_cast<QWidget*>(widget1), manufacturerWindow->windowTitle());
	manufacturerWindow->show();

	connect(manufacturerWindow,	&cManufacturerWindow::somethingChanged,	this,				&cMainWindow::onSomethingChanged);
	connect(this,				&cMainWindow::manufacturerNameChanged,	manufacturerWindow,	&cManufacturerWindow::onManufacturerChanged);
	connect(manufacturerWindow,	&cManufacturerWindow::subWindowClosed,	this,				&cMainWindow::onSubWindowClosed);
}

void cMainWindow::removeManufacturer()
{
	if(!ui->m_manufacturerList->selectionModel()->selectedIndexes().count())
		return;

	QModelIndex				index				= ui->m_manufacturerList->selectionModel()->selectedIndexes()[0];
	QStandardItem*			item				= m_manufacturerListModel->itemFromIndex(index);
	cManufacturer*			manufacturer		= item->data(ITEM_MANUFACTURER_DATA).value<cManufacturer*>();

	if(QMessageBox::question(this, tr("Remove Manufacturer"), QString(tr("Are you sure you want to remove \"%1\"?")).arg(manufacturer->name())) == QMessageBox::No)
		return;

	if(!manufacturer->remove())
		return;

	m_manufacturerListModel->removeRow(index.row());
}

void cMainWindow::addDistributor()
{
	bool	ok;
	QString			name			= QInputDialog::getText(this, tr("New Distributor"), tr("Name:"), QLineEdit::Normal, "", &ok);
	if(!ok)
		return;

	cDistributor*	distributor	= m_distributorList->add(name);

	QStandardItem*	item			= new QStandardItem(name);
	item->setData(QVariant::fromValue(distributor), ITEM_DISTRIBUTOR_DATA);
	m_distributorListModel->appendRow(item);
	m_distributorListModel->sort(Qt::AscendingOrder | Qt::CaseInsensitive);

	onSomethingChanged();
}

void cMainWindow::editDistributor()
{
	if(!ui->m_distributorList->selectionModel()->selectedIndexes().count())
		return;

	QModelIndex				index				= ui->m_distributorList->selectionModel()->selectedIndexes()[0];
	QStandardItem*			item				= m_distributorListModel->itemFromIndex(index);
	cDistributor*			distributor			= item->data(ITEM_DISTRIBUTOR_DATA).value<cDistributor*>();

	for(int x = 0;x < ui->m_mainTab->count();x++)
	{
		cWidget*	widget	= static_cast<cWidget*>(ui->m_mainTab->widget(x));
		if(widget->type() == cWidget::TYPE_distributor)
		{
			cDistributorWindow*	distributorWindow	= static_cast<cDistributorWindow*>(widget->widget());
			if(distributorWindow->distributor() == distributor)
			{
				ui->m_mainTab->setCurrentIndex(x);
				ui->m_mdiArea->setActiveSubWindow(widget->window());
				m_updatingTab	= false;
				return;
			}
		}
	}

	cDistributorWindow*	distributorWindow	= new cDistributorWindow(this);
	distributorWindow->setDistributor(distributor);
	cWidget*				widget1				= new cWidget(distributorWindow);
	widget1->setWindow(ui->m_mdiArea->addSubWindow(distributorWindow));
	ui->m_mainTab->addTab(static_cast<QWidget*>(widget1), distributorWindow->windowTitle());
	distributorWindow->show();

	connect(distributorWindow,	&cDistributorWindow::somethingChanged,	this,				&cMainWindow::onSomethingChanged);
	connect(this,				&cMainWindow::distributorNameChanged,	distributorWindow,	&cDistributorWindow::onDistributorChanged);
	connect(distributorWindow,	&cDistributorWindow::subWindowClosed,	this,				&cMainWindow::onSubWindowClosed);
}

void cMainWindow::removeDistributor()
{
	if(!ui->m_distributorList->selectionModel()->selectedIndexes().count())
		return;

	QModelIndex				index				= ui->m_distributorList->selectionModel()->selectedIndexes()[0];
	QStandardItem*			item				= m_distributorListModel->itemFromIndex(index);
	cDistributor*			distributor			= item->data(ITEM_DISTRIBUTOR_DATA).value<cDistributor*>();

	if(QMessageBox::question(this, tr("Remove distributor"), QString(tr("Are you sure you want to remove \"%1\"?")).arg(distributor->name())) == QMessageBox::No)
		return;

	if(!distributor->remove())
		return;

	m_distributorListModel->removeRow(index.row());
}
