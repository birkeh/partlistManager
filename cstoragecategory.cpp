#include "cstoragecategory.h"
#include "cstorage.h"

#include <QSqlQuery>
#include <QSqlError>

#include <QStandardItem>

#include "common.h"

#include <QDebug>


bool sortAsc(cStorageCategory* &v1, cStorageCategory* &v2)
{
	return(v1->name() < v2->name());
}


cStorageCategory::cStorageCategory(cDatabase* db, qint32 id, QObject* parent) :
	QObject(parent),
	m_db(db),
	m_changed(false),
	m_id(id),
	m_parent(-1),
	m_name(""),
	m_description(""),
	m_parentCategory(nullptr),
	m_list(nullptr),
	m_childList(new cStorageCategoryList(nullptr)),
	m_storageList(new cStorageList(db, nullptr, nullptr)),
	m_item(nullptr)
{
}

cStorageCategory::cStorageCategory(cStorageCategory* storageCategory)
{
	set(storageCategory);
}

void cStorageCategory::set(cStorageCategory* storageCategory)
{
	m_db			= storageCategory->db();
	m_id			= storageCategory->id();
	m_parent		= storageCategory->parent();
	m_name			= storageCategory->name();
	m_description	= storageCategory->description();
}

void cStorageCategory::setID(const qint32& id)
{
	m_id		= id;
	m_changed	= true;
	emit idChanged(id);
	emit storageCategoryChanged(this);
}

qint32 cStorageCategory::id()
{
	return(m_id);
}

void cStorageCategory::setParent(const qint32& parent)
{
	m_parent	= parent;
	m_changed	= true;
	emit parentChanged(parent);
	emit storageCategoryChanged(this);
}

qint32 cStorageCategory::parent()
{
	return(m_parent);
}

void cStorageCategory::addStorage(cStorage* storage)
{
	m_storageList->add(storage);
}

cStorageList* cStorageCategory::storageList()
{
	return(m_storageList);
}

void cStorageCategory::setName(const QString& name)
{
	m_name		= name;
	m_changed	= true;
	emit nameChanged(name);
	emit storageCategoryChanged(this);
}

QString cStorageCategory::name()
{
	return(m_name);
}

void cStorageCategory::setDescription(const QString &description)
{
	m_description	= description;
	m_changed		= true;
	emit descriptionChanged(description);
	emit storageCategoryChanged(this);
}

QString cStorageCategory::description()
{
	return(m_description);
}

void cStorageCategory::setParentCategory(cStorageCategory* parentCategory)
{
	m_parentCategory	= parentCategory;
}

cStorageCategory* cStorageCategory::parentCategory()
{
	return(m_parentCategory);
}

void cStorageCategory::setItem(QStandardItem* item)
{
	m_item		= item;
}

QStandardItem* cStorageCategory::item()
{
	return(m_item);
}

cStorageCategoryList* cStorageCategory::childList()
{
    return(m_childList);
}

cDatabase* cStorageCategory::db()
{
    return(m_db);
}

bool cStorageCategory::save()
{
	if(!m_changed)
		return(true);

	QSqlQuery	query;

	if(m_id == -1)
		query.prepare("INSERT INTO storage_category (parent, name, description) VALUES (:parent, :name, :description);");
	else
		query.prepare("UPDATE storage_category SET parent=:parent, name=:name, description=:description WHERE id=:id;");

	query.bindValue(":id", m_id);
	query.bindValue(":parent", (m_parent==-1)?QVariant(QVariant::Int):m_parent);
	query.bindValue(":name", m_name);
	query.bindValue(":description", m_description);

	if(!query.exec())
	{
		myDebug << query.lastError().text();
		return(false);
	}

	if(m_id == -1)
	{
		if(!query.exec("SELECT MAX(id) MAXID FROM storage_category;"))
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

bool cStorageCategory::remove()
{
	QSqlQuery	query;

	query.prepare("DELETE FROM storage_category WHERE id=:id;");
	query.bindValue(":id", m_id);

	if(!query.exec())
	{
		myDebug << query.lastError().text();
		return(false);
	}

	return(true);
}

void cStorageCategory::clearSave()
{
	m_changed	= false;
}

cStorageCategoryList::cStorageCategoryList(cDatabase* db, QObject* parent) :
	QObject(parent),
	m_db(db)
{
}

bool cStorageCategoryList::load()
{
	if(!m_db->db().isOpen())
		return(false);

	QString		sql	= QString("SELECT   id, "
							  "         parent, "
							  "         name, "
							  "         description "
							  "FROM     storage_category "
							  "ORDER BY parent, "
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
		cStorageCategory*	lpCategory	= new cStorageCategory(m_db, query.value("id").toInt());
		if(query.value("parent").isNull())
			lpCategory->setParent(-1);
		else
			lpCategory->setParent(query.value("parent").toInt());
		lpCategory->setName(query.value("name").toString());
		lpCategory->setDescription(query.value("description").toString());

		cStorageCategory*	lpParent	= find(lpCategory->parent());

		if(lpParent)
		{
			lpCategory->setParentCategory(lpParent);
			lpParent->childList()->add(lpCategory);
		}
		else
			add(lpCategory);
	}

	sort();

	return(true);
}

cStorageCategory* cStorageCategoryList::add(const qint32& id)
{
	cStorageCategory*	lpNew	= find(id);

	if(!lpNew)
	{
		lpNew	= new cStorageCategory(m_db, id);
		append(lpNew);
		emit storageCategoryAdded(lpNew);
	}

	return(lpNew);
}

cStorageCategory* cStorageCategoryList::add(const QString& name)
{
	cStorageCategory*	lpNew	= new cStorageCategory(m_db);
	lpNew->setName(name);
	append(lpNew);
	emit storageCategoryAdded(lpNew);

	return(lpNew);
}

void cStorageCategoryList::add(cStorageCategory* storageCategory)
{
	append(storageCategory);
}

bool cStorageCategoryList::remove(const qint32& id)
{
	cStorageCategory*	storageCategory	= find(id);
	if(!storageCategory)
		return(false);

	if(!storageCategory->remove())
		return(false);

	if(!removeOne(storageCategory))
		return(false);

	emit storageCategoryRemoved(storageCategory);

	return(true);
}

cStorageCategory* cStorageCategoryList::find(const qint32& id)
{
	for(int i = 0;i < count();i++)
	{
		if(at(i)->id() == id)
			return(at(i));
	}

	cStorageCategory*	category;
	for(int i = 0;i < count();i++)
	{
		category	= at(i)->childList()->find(id);
		if(category)
			return(category);
	}

	return(nullptr);
}

cStorageCategory* cStorageCategoryList::find(const QString& name)
{
	for(int i = 0;i < count();i++)
	{
		if(at(i)->name() == name)
			return(at(i));
	}

	cStorageCategory*	category;
	for(int i = 0;i < count();i++)
	{
		category	= at(i)->childList()->find(name);
		if(category)
			return(category);
	}

	return(nullptr);
}

void cStorageCategoryList::sort()
{
	std::sort(begin(), end(), sortAsc);

	for(int i = 0;i < count();i++)
		at(i)->childList()->sort();
}

bool cStorageCategoryList::fillList(QStandardItemModel* model, QStandardItem* parent)
{
	if(!parent)
		model->clear();

	if(!count())
		return(true);

	for(int i = 0;i < count();i++)
	{
		QStandardItem*	item	= new QStandardItem;
		at(i)->setItem(item);
		item->setText(at(i)->name());
		item->setData(QVariant::fromValue(at(i)), ITEM_STORAGE_CATEGORY_DATA);

		if(parent)
			parent->appendRow(item);
		else
			model->appendRow(item);

		if(at(i)->childList())
			at(i)->childList()->fillList(model, item);
	}

	return(true);
}

bool cStorageCategoryList::save()
{
	bool	ret	= true;

	for(int i = 0;i < count();i++)
	{
		if(!at(i)->save())
			ret	= false;
	}

	return(ret);
}
