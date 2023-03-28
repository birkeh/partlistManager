#include "common.h"
#include "cdatabase.h"

#include <QSettings>

#include <QDebug>


cDatabase::cDatabase()
{
	QSettings	settings;

	m_databaseType	= settings.value("database/databasetype", "").toString();
	m_hostName		= settings.value("database/hostname", "").toString();
	m_port			= settings.value("database/port", "").toInt();
	m_databaseName	= settings.value("database/databaseName").toString();
	m_userName		= settings.value("database/username", "").toString();
	m_password		= settings.value("database/password").toString();

	addTranslate("AUTOINCREMENT", "QMYSQL", "AUTO_INCREMENT");
	addTranslate("AUTOINCREMENT", "QSQLITE", "AUTOINCREMENT");
}

cDatabase::~cDatabase()
{
	if(m_db.isOpen())
		m_db.close();
}

void cDatabase::addTranslate(const QString& sql, const QString& dbType, const QString& translated)
{
	SQLTRANSLATE	st = { sql, dbType, translated };
	m_sqlTranslate.append(st);
}

QString cDatabase::translate(const QString& sql, const QString& dbType)
{
	for(QList<SQLTRANSLATE>::iterator st = m_sqlTranslate.begin();st != m_sqlTranslate.end();st++)
	{
		if(st->sql == sql && st->dbType == dbType)
			return(st->translated);
	}
	return(sql);
}

bool cDatabase::connect()
{
	m_db	= QSqlDatabase::addDatabase(m_databaseType);
	m_db.setHostName(m_hostName);
	m_db.setPort(m_port);
	m_db.setDatabaseName(m_databaseName);
	m_db.setUserName(m_userName);
	m_db.setPassword(m_password);

	if(!m_db.open())
	{
		myDebug << m_db.lastError().text();
		return(false);
	}

	if(!checkDB())
	{
		myDebug << m_db.lastError().text();
		return(false);
	}

	return(true);
}

QSqlDatabase cDatabase::db()
{
	return(m_db);
}

bool cDatabase::checkDB()
{
	QSqlQuery	query(m_db);
	if(!query.exec("SELECT version FROM config;"))
	{
		if(!createTables())
		return(false);

		if(!query.exec("SELECT version FROM config;"))
			return(false);
	}

	query.first();
	QString	version	= query.value("version").toString();
	if(version.compare(APP_VERSION))
	{
		if(!upgradeDB(APP_VERSION))
			return(false);
	}

	return(true);
}

bool cDatabase::createTables()
{
	QSqlQuery	query(m_db);

	// CONFIG
	if(!query.exec("CREATE TABLE config "
				   "( version VARCHAR(32) NOT NULL "
				   ");"))
	{
		myDebug << query.lastError().text();
		return(false);
	}

	if(!query.exec(QString("INSERT INTO config (version) VALUES ('%1');").arg(APP_VERSION)))
	{
		myDebug << query.lastError().text();
		return(false);
	}

	// MANUFACTURER
	if(!query.exec(QString("CREATE TABLE manufacturer "
						   "( id                  INTEGER PRIMARY KEY %1, "
						   "  name                VARCHAR(255) NOT NULL, "
						   "  address             VARCHAR(2000), "
						   "  url                 VARCHAR(255), "
						   "  email               VARCHAR(255), "
						   "  phone               VARCHAR(255), "
						   "  fax                 VARCHAR(255), "
						   "  comment             VARCHAR(2000) "
						   ");").arg(translate("AUTOINCREMENT", m_databaseType))))
	{
		myDebug << query.lastError().text();
		return(false);
	}

	// DISTRIBUTOR
	if(!query.exec(QString("CREATE TABLE distributor "
						   "( id                  INTEGER PRIMARY KEY %1, "
						   "  name                VARCHAR(255) NOT NULL, "
						   "  address             VARCHAR(2000), "
						   "  url                 VARCHAR(255), "
						   "  email               VARCHAR(255), "
						   "  phone               VARCHAR(255), "
						   "  fax                 VARCHAR(255), "
						   "  comment             VARCHAR(2000) "
						   ");").arg(translate("AUTOINCREMENT", m_databaseType))))
	{
		myDebug << query.lastError().text();
		return(false);
	}

	// STORAGE_CATEGORY
	if(!query.exec(QString("CREATE TABLE storage_category "
						   "( id                  INTEGER PRIMARY KEY %1, "
						   "  parent              INTEGER, "
						   "  name                VARCHAR(255) NOT NULL, "
						   "  description         VARCHAR(2000), "
						   "  CONSTRAINT          FK_storage_category_id FOREIGN KEY (parent) REFERENCES storage_category(id) "
						   ");").arg(translate("AUTOINCREMENT", m_databaseType))))
	{
		myDebug << query.lastError().text();
		return(false);
	}

	// STORAGE
	if(!query.exec(QString("CREATE TABLE storage "
						   "( id                  INTEGER PRIMARY KEY %1, "
						   "  storage_category_id INTEGER, "
						   "  name                VARCHAR(255) NOT NULL, "
						   "  description         VARCHAR(2000), "
						   "  CONSTRAINT          FK_storage_storage_category_id FOREIGN KEY (storage_category_id) REFERENCES storage_category(id) "
						   ");").arg(translate("AUTOINCREMENT", m_databaseType))))
	{
		myDebug << query.lastError().text();
		return(false);
	}

	return(true);
}

bool cDatabase::upgradeDB(const QString& /*version*/)
{
	return(true);
}
