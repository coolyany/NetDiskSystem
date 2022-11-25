#include "OperateDB.h"
#include <QDebug>
#include <QMessageBox>

OperateDB *OperateDB::instance = nullptr;

OperateDB::OperateDB(QObject* parent)
	: QObject(parent)
{
	// 连接数据库
	m_db = QSqlDatabase::addDatabase("QSQLITE"); // 连接的数据库类型,这里选择sqlite
	init();
}

OperateDB::~OperateDB()
{
	m_db.close(); // 关闭数据库连接
}

OperateDB* OperateDB::getInstance()
{
	// TODO: 在此处插入 return 语句
	//static OperateDB instance;
	if (instance == NULL)
	{
		instance = new OperateDB();
	}
	return instance;
}

bool OperateDB::UserRegister(const char* username, const char* password)
{
	if (username == NULL || password == NULL)
	{
		return false;
	}

	char insert_userinfo_sql[128] = { '\0' };
	sprintf(insert_userinfo_sql, "INSERT INTO userInfo (name, password) VALUES ('%s','%s')", username, password);

	printf("sql query :: %s\n", insert_userinfo_sql);

	QSqlQuery sql_query;
	return sql_query.exec(insert_userinfo_sql);

}

bool OperateDB::UserLogin(const char * username, const char * password)
{
	if (username == NULL || password == NULL)
	{
		return false;
	}

	char query_userinfo_sql[128] = { '\0' };
	sprintf(query_userinfo_sql, "SELECT * FROM userInfo WHERE name='%s' and password='%s' and online=0", username, password);

	printf("sql query :: %s\n", query_userinfo_sql);

	QSqlQuery sql_query;
	sql_query.exec(query_userinfo_sql);

	//移动到查询结果的下一条记录
	if (sql_query.next())
	{
		char update_userinfo_sql[128] = { '\0' };
		sprintf(update_userinfo_sql, "UPDATE userInfo SET online=1 WHERE name='%s' and password='%s'", username, password);

		return sql_query.exec(update_userinfo_sql);
	}
	else
	{
		return false;
	}
}

QStringList OperateDB::GetUserOnline()
{
	char online_userinfo_sql[128] = { '\0' };
	sprintf(online_userinfo_sql, "SELECT name FROM userInfo WHERE online=1");

	QSqlQuery sql_query;
	sql_query.exec(online_userinfo_sql);
	QStringList userNames;
	while (sql_query.next()) {
		userNames.append(sql_query.value(0).toString());
	}

	return userNames;
}

void OperateDB::setOffline(const char * name)
{
	if (!name) {
		printf("name is NULL\n");
		return;
	}
	//登录状态设置为0，下线状态
	char offline_userinfo_sql[128] = { '\0' };
	sprintf(offline_userinfo_sql, "UPDATE userInfo SET online=0 WHERE name='%s'", name);

	printf("sql query :: %s\n", offline_userinfo_sql);

	QSqlQuery query;
	query.exec(offline_userinfo_sql);
}

bool OperateDB::connectSql(const QString & dbName)
{
	m_db.setHostName("localhost");//本地数据库
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

void OperateDB::init()
{
	//如果打开成功
	if (connectSql("netdisk.db"))
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
				"name VARCHAR(30) UNIQUE,"// 不能重名,唯一
				"password VARCHAR(30),"
				"online INTEGER DEFAULT(0))";
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
				"id INTEGER,"
				"friendId INTEGER,"
				"PRIMARY KEY(id,friendId))";
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
			QString data = QString("%1, %2, %3, %4").arg(sql_query.value(0).toString()).arg(sql_query.value(1).toString())
				.arg(sql_query.value(2).toString()).arg(sql_query.value(3).toString());
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
