#include "cdistributor.h"

#include <QSqlQuery>
#include <QSqlError>

#include <QStandardItem>

#include "common.h"


cDistributor::cDistributor(cDatabase* db, qint32 id, QObject* parent) :
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

cDistributor::cDistributor(cDistributor* distributor)
{
	set(distributor);
}

void cDistributor::set(cDistributor* distributor)
{
	m_db		= distributor->db();
	m_id		= distributor->id();
	m_name		= distributor->name();
	m_address	= distributor->address();
	m_url		= distributor->url();
	m_email		= distributor->email();
	m_comment	= distributor->comment();
	m_phone		= distributor->phone();
	m_fax		= distributor->fax();
}

void cDistributor::setID(const qint32& id)
{
	m_id		= id;
	m_changed	= true;
	emit idChanged(id);
	emit distributorChanged(this);
}

qint32 cDistributor::id()
{
	return(m_id);
}

void cDistributor::setName(const QString& name)
{
	m_name		= name;
	m_changed	= true;
	emit nameChanged(name);
	emit distributorChanged(this);
}

QString cDistributor::name()
{
	return(m_name);
}

void cDistributor::setAddress(const QString& address)
{
	m_address	= address;
	emit addressChanged(address);
	emit distributorChanged(this);
}

QString cDistributor::address()
{
	return(m_address);
}

void cDistributor::setURL(const QString& url)
{
	m_url		= url;
	m_changed	= true;
	emit urlChanged(url);
	emit distributorChanged(this);
}

QString cDistributor::url()
{
	return(m_url);
}

void cDistributor::setEmail(const QString& email)
{
	m_email		= email;
	m_changed	= true;
	emit emailChanged(email);
	emit distributorChanged(this);
}

QString cDistributor::email()
{
	return(m_email);
}

void cDistributor::setComment(const QString& comment)
{
	m_comment	= comment;
	emit commentChanged(comment);
	emit distributorChanged(this);
}

QString cDistributor::comment()
{
	return(m_comment);
}

void cDistributor::setPhone(const QString& phone)
{
	m_phone		= phone;
	m_changed	= true;
	emit phoneChanged(phone);
	emit distributorChanged(this);
}

QString cDistributor::phone()
{
	return(m_phone);
}

void cDistributor::setFax(const QString& fax)
{
	m_fax		= fax;
	m_changed	= true;
	emit faxChanged(fax);
	emit distributorChanged(this);
}

QString cDistributor::fax()
{
	return(m_fax);
}

void cDistributor::setItem(QStandardItem* item)
{
	m_item		= item;
}

QStandardItem* cDistributor::item()
{
	return(m_item);
}

cDatabase* cDistributor::db()
{
    return(m_db);
}

bool cDistributor::save()
{
	if(!m_changed)
		return(true);

	QSqlQuery	query;

	if(m_id == -1)
		query.prepare("INSERT INTO distributor (name, address, url, email, comment, phone, fax) VALUES (:name, :address, :url, :email, :comment, :phone, :fax);");
	else
		query.prepare("UPDATE distributor SET name=:name, address=:address, url=:url, email=:email, comment=:comment, phone=:phone, fax=:fax WHERE id=:id;");

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
		if(!query.exec("SELECT MAX(id) MAXID FROM distributor;"))
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

bool cDistributor::remove()
{
	QSqlQuery	query;

	query.prepare("DELETE FROM distributor WHERE id=:id;");
	query.bindValue(":id", m_id);

	if(!query.exec())
	{
		myDebug << query.lastError().text();
		return(false);
	}

	return(true);
}

void cDistributor::clearSave()
{
	m_changed	= false;
}

cDistributorList::cDistributorList(cDatabase* db, QObject* parent) :
	QObject(parent),
	m_db(db)
{
}

bool cDistributorList::load()
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
							  "FROM     distributor "
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
		cDistributor*	lpDistributor	= add(query.value("id").toInt());
		lpDistributor->setName(query.value("name").toString());
		lpDistributor->setAddress(query.value("address").toString());
		lpDistributor->setURL(query.value("url").toString());
		lpDistributor->setEmail(query.value("email").toString());
		lpDistributor->setComment(query.value("comment").toString());
		lpDistributor->setPhone(query.value("phone").toString());
		lpDistributor->setFax(query.value("fax").toString());
	}

	return(true);
}

cDistributor* cDistributorList::add(const qint32& id)
{
	cDistributor*	lpNew	= find(id);

	if(!lpNew)
	{
		lpNew	= new cDistributor(m_db, id);
		append(lpNew);
		emit distributorAdded(lpNew);
	}

	return(lpNew);
}

cDistributor* cDistributorList::add(const QString& name)
{
	cDistributor*	lpNew	= new cDistributor(m_db);
	lpNew->setName(name);
	append(lpNew);
	emit distributorAdded(lpNew);

	return(lpNew);
}

bool cDistributorList::remove(const qint32& id)
{
	cDistributor*	manufacturer	= find(id);
	if(!manufacturer)
		return(false);

	if(!manufacturer->remove())
		return(false);

	if(!removeOne(manufacturer))
		return(false);

	emit distributorRemoved(manufacturer);

	return(true);
}

cDistributor* cDistributorList::find(const qint32& id)
{
	for(int i = 0;i < count();i++)
	{
		if(at(i)->id() == id)
			return(at(i));
	}

	return(nullptr);
}

cDistributor* cDistributorList::find(const QString& name)
{
	for(int i = 0;i < count();i++)
	{
		if(at(i)->name() == name)
			return(at(i));
	}

	return(nullptr);
}

bool cDistributorList::fillList(QStandardItemModel* model)
{
	model->clear();

	if(!count())
		return(true);

	for(int i = 0;i < count();i++)
	{
		QStandardItem*	item	= new QStandardItem;
		at(i)->setItem(item);
		item->setText(at(i)->name());
		item->setData(QVariant::fromValue(at(i)), ITEM_DISTRIBUTOR_DATA);
		model->appendRow(item);
	}

	return(true);
}

bool cDistributorList::save()
{
	bool	ret	= true;

	for(int i = 0;i < count();i++)
	{
		if(!at(i)->save())
			ret	= false;
	}

	return(ret);
}
