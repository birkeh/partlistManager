#include "cmanufacturerwindow.h"
#include "ui_cmanufacturerwindow.h"

#include "cmainwindow.h"


cManufacturerWindow::cManufacturerWindow(QWidget *parent) :
	cMDISubWindow(parent),
	ui(new Ui::cManufacturerWindow),
	m_manufacturer(nullptr)
{
	ui->setupUi(this);
}

cManufacturerWindow::~cManufacturerWindow()
{
	delete ui;
}

void cManufacturerWindow::setManufacturer(cManufacturer* manufacturer)
{
	m_manufacturer	= manufacturer;

	fillFields();

	connect(ui->m_name,				&QLineEdit::textChanged,		this,	&cManufacturerWindow::onNameChanged);
	connect(ui->m_address,			&QPlainTextEdit::textChanged,	this,	&cManufacturerWindow::onAddressChanged);
	connect(ui->m_url,				&QLineEdit::textChanged,		this,	&cManufacturerWindow::onURLChanged);
	connect(ui->m_email,			&QLineEdit::textChanged,		this,	&cManufacturerWindow::onEmailChanged);
	connect(ui->m_phone,			&QLineEdit::textChanged,		this,	&cManufacturerWindow::onPhoneChanged);
	connect(ui->m_fax,				&QLineEdit::textChanged,		this,	&cManufacturerWindow::onFaxChanged);
	connect(ui->m_comment,			&QPlainTextEdit::textChanged,	this,	&cManufacturerWindow::onCommentChanged);
}

cManufacturer* cManufacturerWindow::manufacturer()
{
	return(m_manufacturer);
}

void cManufacturerWindow::onManufacturerChanged(cManufacturer* manufacturer)
{
	if(m_manufacturer != manufacturer)
		return;

	fillFields();
}

void cManufacturerWindow::onNameChanged(const QString& name)
{
	m_manufacturer->setName(name);

	if(m_manufacturer->item())
		m_manufacturer->item()->setText(name);

	emit somethingChanged();
}

void cManufacturerWindow::onAddressChanged()
{
	m_manufacturer->setAddress(ui->m_address->toPlainText());

	emit somethingChanged();
}

void cManufacturerWindow::onURLChanged(const QString& url)
{
	m_manufacturer->setURL(url);

	emit somethingChanged();
}

void cManufacturerWindow::onEmailChanged(const QString& email)
{
	m_manufacturer->setEmail(email);

	emit somethingChanged();
}

void cManufacturerWindow::onPhoneChanged(const QString& phone)
{
	m_manufacturer->setPhone(phone);

	emit somethingChanged();
}

void cManufacturerWindow::onFaxChanged(const QString& fax)
{
	m_manufacturer->setFax(fax);

	emit somethingChanged();
}

void cManufacturerWindow::onCommentChanged()
{
	m_manufacturer->setComment(ui->m_comment->toPlainText());

	emit somethingChanged();
}

void cManufacturerWindow::fillFields()
{
	ui->m_name->setText(m_manufacturer->name());
	ui->m_address->setPlainText(m_manufacturer->address());
	ui->m_url->setText(m_manufacturer->url());
	ui->m_email->setText(m_manufacturer->email());
	ui->m_phone->setText(m_manufacturer->phone());
	ui->m_fax->setText(m_manufacturer->fax());
	ui->m_comment->setPlainText(m_manufacturer->comment());

	setWindowTitle("manufacturer - " + m_manufacturer->name());
}
