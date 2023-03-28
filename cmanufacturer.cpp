#include "cmanufacturer.h"

#include <QSqlQuery>
#include <QSqlError>

#include <QStandardItem>

#include "common.h"


cManufacturer::cManufacturer(cDatabase* db, qint32 id, QObject* parent) :
	QObject(parent),
	m_db(db),
	m_changed(false),
	m_id(id),
	m_name(""),
	m_address(""),
	m_url(""),
	m_email(""),
	m_comment(""),
	m_phone(""),
	m_fax(""),
	m_item(nullptr)
{
}

cManufacturer::cManufacturer(cManufacturer* manufacturer)
{
	set(manufacturer);
}

void cManufacturer::set(cManufacturer* manufacturer)
{
	m_db		= manufacturer->db();
	m_id		= manufacturer->id();
	m_name		= manufacturer->name();
	m_address	= manufacturer->address();
	m_url		= manufacturer->url();
	m_email		= manufacturer->email();
	m_comment	= manufacturer->comment();
	m_phone		= manufacturer->phone();
	m_fax		= manufacturer->fax();
}

void cManufacturer::setID(const qint32& id)
{
	m_id		= id;
	m_changed	= true;
	emit idChanged(id);
	emit manufacturerChanged(this);
}

qint32 cManufacturer::id()
{
	return(m_id);
}

void cManufacturer::setName(const QString& name)
{
	m_name		= name;
	m_changed	= true;
	emit nameChanged(name);
	emit manufacturerChanged(this);
}

QString cManufacturer::name()
{
	return(m_name);
}

void cManufacturer::setAddress(const QString& address)
{
	m_address	= address;
	emit addressChanged(address);
	emit manufacturerChanged(this);
}

QString cManufacturer::address()
{
	return(m_address);
}

void cManufacturer::setURL(const QString& url)
{
	m_url		= url;
	m_changed	= true;
	emit urlChanged(url);
	emit manufacturerChanged(this);
}

QString cManufacturer::url()
{
	return(m_url);
}

void cManufacturer::setEmail(const QString& email)
{
	m_email		= email;
	m_changed	= true;
	emit emailChanged(email);
	emit manufacturerChanged(this);
}

QString cManufacturer::email()
{
	return(m_email);
}

void cManufacturer::setComment(const QString& comment)
{
	m_comment	= comment;
	emit commentChanged(comment);
	emit manufacturerChanged(this);
}

QString cManufacturer::comment()
{
	return(m_comment);
}

void cManufacturer::setPhone(const QString& phone)
{
	m_phone		= phone;
	m_changed	= true;
	emit phoneChanged(phone);
	emit manufacturerChanged(this);
}

QString cManufacturer::phone()
{
	return(m_phone);
}

void cManufacturer::setFax(const QString& fax)
{
	m_fax		= fax;
	m_changed	= true;
	emit faxChanged(fax);
	emit manufacturerChanged(this);
}

QString cManufacturer::fax()
{
	return(m_fax);
}

void cManufacturer::setItem(QStandardItem* item)
{
	m_item		= item;
}

QStandardItem* cManufacturer::item()
{
	return(m_item);
}

bool cManufacturer::save()
{
	if(!m_changed)
		return(true);

	QSqlQuery	query(m_db->db());

	if(m_id == -1)
		query.prepare("INSERT INTO manufacturer (name, address, url, email, comment, phone, fax) VALUES (:name, :address, :url, :email, :comment, :phone, :fax);");
	else
		query.prepare("UPDATE manufacturer SET name=:name, address=:address, url=:url, email=:email, comment=:comment, phone=:phone, fax=:fax WHERE id=:id;");

	query.bindValue(":id", m_id);
	query.bindValue(":name", m_name);
	query.bindValue(":address", m_address);
	query.bindValue(":url", m_url);
	query.bindValue(":email", m_email);
	query.bindValue(":comment", m_comment);
	query.bindValue(":phone", m_phone);
	query.bindValue(":fax", m_fax);

	if(!query.exec())
	{
		myDebug << query.lastError().text();
		return(false);
	}

	if(m_id == -1)
	{
		if(!query.exec("SELECT MAX(id) MAXID FROM manufacturer;"))
		{
			myDebug << query.lastError().text();
			return(false);
		}

		query.first();
		m_id = query.value("MAXID").toInt();
	}
	m_changed	= false;
	return(true);
}

cDatabase* cManufacturer::db()
{
    return(m_db);
}

bool cManufacturer::remove()
{
	QSqlQuery	query(m_db->db());

	query.prepare("DELETE FROM manufacturer WHERE id=:id;");
	query.bindValue(":id", m_id);

	if(!query.exec())
	{
		myDebug << query.lastError().text();
		return(false);
	}

	return(true);
}

void cManufacturer::clearSave()
{
	m_changed	= false;
}

cManufacturerList::cManufacturerList(cDatabase* db, QObject* parent) :
	QObject(parent),
	m_db(db)
{
}

bool cManufacturerList::load()
{
	if(!m_db->db().isOpen())
		return(false);

	QString		sql	= QString("SELECT   id, "
							  "         name, "
							  "         address, "
							  "         url, "
							  "         email, "
							  "         comment, "
							  "         phone, "
							  "         fax "
							  "FROM     manufacturer "
							  "ORDER BY name;");
	QSqlQuery	query(m_db->db());

	query.prepare(sql);

	if(!query.exec())
	{
		myDebug << query.lastError().text();
		return(false);
	}

	while(query.next())
	{
		cManufacturer*	lpManufacturer	= add(query.value("id").toInt());
		lpManufacturer->setName(query.value("name").toString());
		lpManufacturer->setAddress(query.value("address").toString());
		lpManufacturer->setURL(query.value("url").toString());
		lpManufacturer->setEmail(query.value("email").toString());
		lpManufacturer->setComment(query.value("comment").toString());
		lpManufacturer->setPhone(query.value("phone").toString());
		lpManufacturer->setFax(query.value("fax").toString());
	}

	return(true);
}

cManufacturer* cManufacturerList::add(const qint32& id)
{
	cManufacturer*	lpNew	= find(id);

	if(!lpNew)
	{
		lpNew	= new cManufacturer(m_db, id);
		append(lpNew);
		emit manufacturerAdded(lpNew);
	}

	return(lpNew);
}

cManufacturer* cManufacturerList::add(const QString& name)
{
	cManufacturer*	lpNew	= new cManufacturer(m_db);
	lpNew->setName(name);
	append(lpNew);
	emit manufacturerAdded(lpNew);

	return(lpNew);
}

bool cManufacturerList::remove(const qint32& id)
{
	cManufacturer*	manufacturer	= find(id);
	if(!manufacturer)
		return(false);

	if(!manufacturer->remove())
		return(false);

	if(!removeOne(manufacturer))
		return(false);

	emit manufacturerRemoved(manufacturer);

	return(true);
}

cManufacturer* cManufacturerList::find(const qint32& id)
{
	for(int i = 0;i < count();i++)
	{
		if(at(i)->id() == id)
			return(at(i));
	}

	return(nullptr);
}

cManufacturer* cManufacturerList::find(const QString& name)
{
	for(int i = 0;i < count();i++)
	{
		if(at(i)->name() == name)
			return(at(i));
	}

	return(nullptr);
}

bool cManufacturerList::fillList(QStandardItemModel* model)
{
	model->clear();

	if(!count())
		return(true);

	for(int i = 0;i < count();i++)
	{
		QStandardItem*	item	= new QStandardItem;
		at(i)->setItem(item);
		item->setText(at(i)->name());
		item->setData(QVariant::fromValue(at(i)), ITEM_MANUFACTURER_DATA);
		model->appendRow(item);
	}

	return(true);
}

bool cManufacturerList::save()
{
	bool	ret	= true;

	for(int i = 0;i < count();i++)
	{
		if(!at(i)->save())
			ret	= false;
	}

	return(ret);
}
