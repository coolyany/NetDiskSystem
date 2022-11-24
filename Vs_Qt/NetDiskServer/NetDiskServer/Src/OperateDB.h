#pragma once
#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

class OperateDB : public QObject
{
	Q_OBJECT
public:
	~OperateDB();
	static OperateDB* getInstance(); // 公用获取引用，实现单例模式

	bool UserRegister(const char* username, const char* password);

private:
	explicit OperateDB(QObject* parent = Q_NULLPTR);
	void init();//初始化
	bool connectSql(const QString &dbName);

	QSqlDatabase m_db; // 连接数据库
	static OperateDB *instance;

};