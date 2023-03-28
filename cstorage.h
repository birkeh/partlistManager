#ifndef CSTORAGE_H
#define CSTORAGE_H


#include "cdatabase.h"
#include "cstoragecategory.h"

#include <QMetaType>
#include <QList>
#include <QObject>

#include <QStandardItem>
#include <QStandardItemModel>


class cStorage : public QObject
{
	Q_OBJECT
public:
	explicit cStorage(cDatabase* db, qint32 id = -1, QObject *parent = nullptr);
	cStorage(cStorage* storage);

	void					set(cStorage* storage);

	void					setID(const qint32& id);
	qint32					id();

	void					setStorageCategory(cStorageCategory* storageCategory);
	cStorageCategory*		storageCategory();

	void					setName(const QString& name);
	QString					name();

	void					setDescription(const QString& description);
	QString					description();

	void					setItem(QStandardItem* item);
	QStandardItem*			item();

	bool					save();
	void					clearSave();
	bool					remove();

	cDatabase*				db();
private:
	cDatabase*				m_db;
	bool					m_changed;
	qint32					m_id;
	cStorageCategory*		m_storageCategory;
	QString					m_name;
	QString					m_description;
	QStandardItem*			m_item;
signals:
	void					storageChanged(cStorage* storage);
	void					idChanged(const qint32& id);
	void					storageCategoryChanged(cStorageCategory* storageCategory);
	void					nameChanged(const QString& name);
	void					descriptionChanged(const QString& description);
};

Q_DECLARE_METATYPE(cStorage*)

class cStorageList : public QObject, public QList<cStorage*>
{
	Q_OBJECT

public:
	cStorageList(cDatabase* db, cStorageCategoryList* storageCategoryList, QObject* parent = nullptr);

	bool					load();
	cStorage*				add(const qint32& id);
	cStorage*				add(const QString& name);
	void					add(cStorage* storage);
	bool					remove(const qint32& id);
	cStorage*				find(const qint32& id);
	cStorage*				find(const QString& name);

	void					sort();

	bool					fillList();

	bool					save();
private:
	cDatabase*				m_db;
	cStorageCategoryList*	m_storageCategoryList;

signals:
	void					storageAdded(cStorage* storage);
	void					storageRemoved(cStorage* storage);
};

#endif // CSTORAGE_H
