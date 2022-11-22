#include "MySql.h"
#include <QDebug>
#include <QMessageBox>

MySql::MySql(QObject* parent)
	: QObject(parent)
{
	// 连接数据库
	m_db = QSqlDatabase::addDatabase("QSQLITE"); // 连接的数据库类型,这里选择sqlite
}

MySql::~MySql()
{
	m_db.close(); // 关闭数据库连接
}

MySql & MySql::getInstance()
{
	// TODO: 在此处插入 return 语句
	static MySql instance;
	return instance;
}

bool MySql::connectSql(const QString & dbName)
{
	m_db.setDatabaseName(dbName);
	//m_db.setDatabaseName("netdisksystem.db");
	m_db.setUserName("root");
	m_db.setPassword("123456");
	if (!m_db.open()) {
		QMessageBox::critical(0, QObject::tr("Database Error"),
			m_db.lastError().text());
		return false;
	}
	return true;
}

void MySql::init()
{
	//如果打开成功
	if (connectSql("netdisksystem.db"))
	{
		qDebug() << "open successful";

		QSqlQuery sql_query;

		

		//QSqlQuery query;
		bool re = sql_query.exec("select * from userInfo");
		//若不存在userInfo表，则创建一张
		if (!re)
		{
			//创建用户信息表,id为主键（自动增长）
			QString create_userinfo_sql = "CREATE TABLE userInfo("
				"id INTEGER PRIMARY KEY AUTOINCREMENT,"
				"name VARCHAR(30),"
				"password VARCHAR(30))";
			sql_query.prepare(create_userinfo_sql);
			if (!sql_query.exec())
			{
				qDebug() << sql_query.lastError();
			}
			else
			{
				qDebug() << "table created!";
			}
		}

		re = sql_query.exec("select * from userFriendInfo");
		//若不存在userInfo表，则创建一张
		if (!re)
		{
			//创建用户信息表,id为主键（自动增长）
			QString create_userFriendInfo_sql = "CREATE TABLE userFriendInfo("
				"id INTEGER PRIMARY KEY AUTOINCREMENT,"
				"friendId INTEGER)";
			sql_query.prepare(create_userFriendInfo_sql);
			if (!sql_query.exec())
			{
				qDebug() << sql_query.lastError();
			}
			else
			{
				qDebug() << "table created!";
			}
		}
		
		//查询
		re = sql_query.exec("select * from userInfo");
		while (sql_query.next())
		{
			QString data = QString("%1, %2, %3").arg(sql_query.value(0).toString()).arg(sql_query.value(1).toString())
				.arg(sql_query.value(2).toString());
			qDebug() << data;
		}

		re = sql_query.exec("select * from userFriendInfo");
		while (sql_query.next())
		{
			QString data = QString("%1, %2").arg(sql_query.value(0).toString()).arg(sql_query.value(1).toString());
			qDebug() << data;
		}
	}
}
