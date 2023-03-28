#ifndef CDISTRIBUTOR_H
#define CDISTRIBUTOR_H


#include "cdatabase.h"

#include <QMetaType>
#include <QList>
#include <QObject>

#include <QStandardItem>
#include <QStandardItemModel>


class cDistributor : public QObject
{
	Q_OBJECT

public:
	explicit cDistributor(cDatabase* db, qint32 id = -1, QObject* parent = nullptr);
	cDistributor(cDistributor* distributor);

	void			set(cDistributor* distributor);

	void			setID(const qint32& id);
	qint32			id();

	void			setName(const QString& name);
	QString			name();

	void			setAddress(const QString& address);
	QString			address();

	void			setURL(const QString& url);
	QString			url();

	void			setEmail(const QString& email);
	QString			email();

	void			setComment(const QString& comment);
	QString			comment();

	void			setPhone(const QString& phone);
	QString			phone();

	void			setFax(const QString& fax);
	QString			fax();

	void			setItem(QStandardItem* item);
	QStandardItem*	item();

	bool			save();
	void			clearSave();
	bool			remove();

	cDatabase*		db();
private:
	cDatabase*		m_db;
	bool			m_changed;
	qint32			m_id;
	QString			m_name;
	QString			m_address;
	QString			m_url;
	QString			m_email;
	QString			m_comment;
	QString			m_phone;
	QString			m_fax;
	QStandardItem*	m_item;
signals:
	void			distributorChanged(cDistributor* distributor);
	void			idChanged(const qint32& id);
	void			nameChanged(const QString& name);
	void			addressChanged(const QString& address);
	void			urlChanged(const QString& url);
	void			emailChanged(const QString& email);
	void			commentChanged(const QString& comment);
	void			phoneChanged(const QString& phone);
	void			faxChanged(const QString& fax);
};

Q_DECLARE_METATYPE(cDistributor*)

class cDistributorList : public QObject, public QList<cDistributor*>
{
	Q_OBJECT

public:
	cDistributorList(cDatabase* db, QObject* parent = nullptr);

	bool			load();
	cDistributor*	add(const qint32& id);
	cDistributor*	add(const QString& name);
	bool			remove(const qint32& id);
	cDistributor*	find(const qint32& id);
	cDistributor*	find(const QString& name);

	bool			fillList(QStandardItemModel* model);

	bool			save();
private:
	cDatabase*		m_db;

signals:
	void			distributorAdded(cDistributor* manufacturer);
	void			distributorRemoved(cDistributor* manufacturer);
};

#endif // CDISTRIBUTOR_H
