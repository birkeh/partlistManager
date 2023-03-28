#ifndef CSTORAGECATEGORY_H
#define CSTORAGECATEGORY_H


#include "cdatabase.h"

#include <QMetaType>
#include <QList>
#include <QObject>

#include <QStandardItem>
#include <QStandardItemModel>


class cStorageCategoryList;
class cStorage;
class cStorageList;

class cStorageCategory : public QObject
{
	Q_OBJECT
public:
	explicit cStorageCategory(cDatabase* db, qint32 id = -1, QObject *parent = nullptr);
	cStorageCategory(cStorageCategory* storageCategory);

	void					set(cStorageCategory* storageCategory);

	void					setID(const qint32& id);
	qint32					id();

	void					setParent(const qint32& parent);
	qint32					parent();

	void					setName(const QString& name);
	QString					name();

	void					setDescription(const QString& description);
	QString					description();

	void					setParentCategory(cStorageCategory* parentCategory);
	cStorageCategory*		parentCategory();

	void					addStorage(cStorage* storage);
	cStorageList*			storageList();

	void					setItem(QStandardItem* item);
	QStandardItem*			item();

	bool					save();
	void					clearSave();
	bool					remove();

	void					setList(cStorageCategoryList* list);
	cStorageCategoryList*	list();

	cStorageCategoryList*	childList();

	cDatabase*				db();
private:
	cDatabase*				m_db;
	bool					m_changed;
	qint32					m_id;
	qint32					m_parent;
	QString					m_name;
	QString					m_description;
	cStorageCategory*		m_parentCategory;
	cStorageCategoryList*	m_list;
	cStorageCategoryList*	m_childList;
	cStorageList*			m_storageList;
	QStandardItem*			m_item;
signals:
	void					storageCategoryChanged(cStorageCategory* storageCategory);
	void					idChanged(const qint32& id);
	void					parentChanged(const qint32& parent);
	void					nameChanged(const QString& name);
	void					descriptionChanged(const QString& description);
};

Q_DECLARE_METATYPE(cStorageCategory*)

class cStorageCategoryList : public QObject, public QList<cStorageCategory*>
{
	Q_OBJECT

public:
	cStorageCategoryList(cDatabase* db, QObject* parent = nullptr);

	bool					load();
	cStorageCategory*		add(const qint32& id);
	cStorageCategory*		add(const QString& name);
	void					add(cStorageCategory* storageCategory);
	bool					remove(const qint32& id);
	cStorageCategory*		find(const qint32& id);
	cStorageCategory*		find(const QString& name);

	void					sort();

	bool					fillList(QStandardItemModel* model, QStandardItem* parent = 0);

	bool					save();
private:
	cDatabase*				m_db;

signals:
	void					storageCategoryAdded(cStorageCategory* storageCategory);
	void					storageCategoryRemoved(cStorageCategory* storageCategory);
};

#endif // CSTORAGECATEGORY_H
