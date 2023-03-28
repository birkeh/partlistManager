#include "cstorage.h"

#include <QSqlQuery>
#include <QSqlError>

#include <QStandardItem>

#include "common.h"

#include <QDebug>


bool sortAsc(cStorage* &v1, cStorage* &v2)
{
	return(v1->name() < v2->name());
}


cStorage::cStorage(cDatabase* db, qint32 id, QObject *parent) :
	QObject(parent),
	m_db(db),
	m_changed(false),
	m_id(id),
	m_storageCategory(nullptr),
	m_name(""),
	m_description(""),
	m_item(nullptr)
{
}

cStorage::cStorage(cStorage* storage)
{
	set(storage);
}

void cStorage::set(cStorage* storage)
{
	m_db				= storage->db();
	m_id				= storage->id();
	m_storageCategory	= storage->storageCategory();
	m_name				= storage->name();
	m_description		= storage->description();
}

void cStorage::setID(const qint32& id)
{
	m_id	= id;
	m_changed	= true;
	emit idChanged(id);
	emit storageChanged(this);
}

qint32 cStorage::id()
{
	return(m_id);
}

void cStorage::setStorageCategory(cStorageCategory* storageCategory)
{
	m_storageCategory	= storageCategory;
	m_changed	= true;
	emit storageCategoryChanged(storageCategory);
	emit storageChanged(this);
}

cStorageCategory* cStorage::storageCategory()
{
	return(m_storageCategory);
}

void cStorage::setName(const QString& name)
{
	m_name	= name;
	m_changed	= true;
	emit nameChanged(name);
	emit storageChanged(this);
}

QString cStorage::name()
{
	return(m_name);
}

void cStorage::setDescription(const QString& description)
{
	m_description	= description;
	m_changed	= true;
	emit descriptionChanged(description);
	emit storageChanged(this);
}

QString cStorage::description()
{
	return(m_description);
}

void cStorage::setItem(QStandardItem* item)
{
	m_item	= item;
}

QStandardItem* cStorage::item()
{
	return(m_item);
}

bool cStorage::save()
{
	if(!m_changed)
		return(true);

	QSqlQuery	query;

	if(m_id == -1)
		query.prepare("INSERT INTO storage (storageCategory, name, description) VALUES (:storageCategory, :name, :description);");
	else
		query.prepare("UPDATE storage SET storageCategory=:storageCategory, name=:name, description=:description WHERE id=:id;");

	query.bindValue(":id", m_id);
	query.bindValue(":storageCategory", m_storageCategory->id());
	query.bindValue(":name", m_name);
	query.bindValue(":description", m_description);

	if(!query.exec())
	{
		myDebug << query.lastError().text();
		return(false);
	}

	if(m_id == -1)
	{
		if(!query.exec("SELECT MAX(id) MAXID FROM storage;"))
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

void cStorage::clearSave()
{
	m_changed	= false;
}

bool cStorage::remove()
{
	QSqlQuery	query;

	query.prepare("DELETE FROM storage WHERE id=:id;");
	query.bindValue(":id", m_id);

	if(!query.exec())
	{
		myDebug << query.lastError().text();
		return(false);
	}

	return(true);
}

cDatabase* cStorage::db()
{
	return(m_db);
}

cStorageList::cStorageList(cDatabase* db, cStorageCategoryList* storageCategoryList, QObject* parent) :
	QObject(parent),
	m_db(db),
	m_storageCategoryList(storageCategoryList)
{
}

bool cStorageList::load()
{
	if(!m_db->db().isOpen())
		return(false);

	QString		sql	= QString("SELECT   id, "
							  "         storage_category_id, "
							  "         name, "
							  "         description "
							  "FROM     storage "
							  "ORDER BY name, "
							  "         id;");
	QSqlQuery	query(m_db->db());

	query.prepare(sql);

	if(!query.exec())
	{
		myDebug << query.lastError().text();
		return(false);
	}

	while(query.next())
	{
		cStorage*	lpStorage	= new cStorage(m_db, query.value("id").toInt());
		cStorageCategory*	storageCatgegory	= m_storageCategoryList->find(query.value("storage_category_id").toInt());
		lpStorage->setStorageCategory(storageCatgegory);
		lpStorage->setName(query.value("name").toString());
		lpStorage->setDescription(query.value("description").toString());
		storageCatgegory->addStorage(lpStorage);

		add(lpStorage);
	}

	sort();

	return(true);
}

cStorage* cStorageList::add(const qint32& id)
{
	cStorage*	lpNew	= find(id);

	if(!lpNew)
	{
		lpNew	= new cStorage(m_db, id);
		append(lpNew);
		emit storageAdded(lpNew);
	}

	return(lpNew);
}

cStorage* cStorageList::add(const QString& name)
{
	cStorage*	lpNew	= new cStorage(m_db);
	lpNew->setName(name);
	append(lpNew);
	emit storageAdded(lpNew);

	return(lpNew);
}

void cStorageList::add(cStorage* storage)
{
	append(storage);
}

bool cStorageList::remove(const qint32& id)
{
	cStorage*	storage	= find(id);
	if(!storage)
		return(false);

	if(!storage->remove())
		return(false);

	if(!removeOne(storage))
		return(false);

	emit storageRemoved(storage);

	return(true);
}

cStorage* cStorageList::find(const qint32& id)
{
	for(int i = 0;i < count();i++)
	{
		if(at(i)->id() == id)
			return(at(i));
	}

	return(nullptr);
}

cStorage* cStorageList::find(const QString& name)
{
	for(int i = 0;i < count();i++)
	{
		if(at(i)->name() == name)
			return(at(i));
	}

	return(nullptr);
}

void cStorageList::sort()
{
	std::sort(begin(), end(), sortAsc);
}

bool cStorageList::fillList()
{
	if(!count())
		return(true);

	QStandardItem	i;
	QFont			font	= i.font();
	font.setItalic(true);
	font.setBold(true);

	for(int i = 0;i < count();i++)
	{
		QStandardItem*	item	= new QStandardItem;
		at(i)->setItem(item);
		item->setText(at(i)->name());
		item->setData(QVariant::fromValue(at(i)), ITEM_STORAGE_DATA);
		item->setFont(font);

		at(i)->storageCategory()->item()->appendRow(item);
	}

	return(true);
}

bool cStorageList::save()
{
	bool	ret	= true;

	for(int i = 0;i < count();i++)
	{
		if(!at(i)->save())
			ret	= false;
	}

	return(ret);
}
