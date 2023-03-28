#ifndef CDATABASE_H
#define CDATABASE_H


#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QList>


typedef struct tagSQLTRANSLATE
{
	QString		sql;
	QString		dbType;
	QString		translated;
} SQLTRANSLATE;


class cDatabase
{
public:
	cDatabase();
	~cDatabase();

	bool				connect();

	QSqlDatabase		db();
private:
	QString				m_databaseType;
	QString				m_hostName;
	qint16				m_port;
	QString				m_databaseName;
	QString				m_userName;
	QString				m_password;
	QSqlDatabase		m_db;

	QList<SQLTRANSLATE>	m_sqlTranslate;

	void				addTranslate(const QString& sql, const QString& dbType, const QString& translated);
	QString				translate(const QString& sql, const QString& dbType);

	bool				checkDB();
	bool				createTables();
	bool				upgradeDB(const QString& version);
};

#endif // CDATABASE_H
