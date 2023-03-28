#include "cdistributorwindow.h"
#include "ui_cdistributorwindow.h"

#include "cmainwindow.h"


cDistributorWindow::cDistributorWindow(QWidget *parent) :
	cMDISubWindow(parent),
	ui(new Ui::cDistributorWindow),
	m_distributor(nullptr)
{
	ui->setupUi(this);
}

cDistributorWindow::~cDistributorWindow()
{
	delete ui;
}

void cDistributorWindow::setDistributor(cDistributor* distributor)
{
	m_distributor	= distributor;

	fillFields();

	connect(ui->m_name,				&QLineEdit::textChanged,		this,	&cDistributorWindow::onNameChanged);
	connect(ui->m_address,			&QPlainTextEdit::textChanged,	this,	&cDistributorWindow::onAddressChanged);
	connect(ui->m_url,				&QLineEdit::textChanged,		this,	&cDistributorWindow::onURLChanged);
	connect(ui->m_email,			&QLineEdit::textChanged,		this,	&cDistributorWindow::onEmailChanged);
	connect(ui->m_phone,			&QLineEdit::textChanged,		this,	&cDistributorWindow::onPhoneChanged);
	connect(ui->m_fax,				&QLineEdit::textChanged,		this,	&cDistributorWindow::onFaxChanged);
	connect(ui->m_comment,			&QPlainTextEdit::textChanged,	this,	&cDistributorWindow::onCommentChanged);
}

cDistributor* cDistributorWindow::distributor()
{
	return(m_distributor);
}

void cDistributorWindow::onDistributorChanged(cDistributor* distributor)
{
	if(m_distributor != distributor)
		return;

	fillFields();
}

void cDistributorWindow::onNameChanged(const QString& name)
{
	m_distributor->setName(name);

	if(m_distributor->item())
		m_distributor->item()->setText(name);

	emit somethingChanged();
}

void cDistributorWindow::onAddressChanged()
{
	m_distributor->setAddress(ui->m_address->toPlainText());

	emit somethingChanged();
}

void cDistributorWindow::onURLChanged(const QString& url)
{
	m_distributor->setURL(url);

	emit somethingChanged();
}

void cDistributorWindow::onEmailChanged(const QString& email)
{
	m_distributor->setEmail(email);

	emit somethingChanged();
}

void cDistributorWindow::onPhoneChanged(const QString& phone)
{
	m_distributor->setPhone(phone);

	emit somethingChanged();
}

void cDistributorWindow::onFaxChanged(const QString& fax)
{
	m_distributor->setFax(fax);

	emit somethingChanged();
}

void cDistributorWindow::onCommentChanged()
{
	m_distributor->setComment(ui->m_comment->toPlainText());

	emit somethingChanged();
}

void cDistributorWindow::fillFields()
{
	ui->m_name->setText(m_distributor->name());
	ui->m_address->setPlainText(m_distributor->address());
	ui->m_url->setText(m_distributor->url());
	ui->m_email->setText(m_distributor->email());
	ui->m_phone->setText(m_distributor->phone());
	ui->m_fax->setText(m_distributor->fax());
	ui->m_comment->setPlainText(m_distributor->comment());

	setWindowTitle("distributor - " + m_distributor->name());
}
