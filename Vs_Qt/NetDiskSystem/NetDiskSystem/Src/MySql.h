#pragma once
#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

class MySql : public QObject
{
	Q_OBJECT
public:
	explicit MySql(QObject* parent = Q_NULLPTR);
	~MySql();
	static MySql& getInstance(); // 公用获取引用，实现单例模式

	bool connectSql(const QString &dbName);
	void init();//初始化
private:
	QSqlDatabase m_db; // 连接数据库
};