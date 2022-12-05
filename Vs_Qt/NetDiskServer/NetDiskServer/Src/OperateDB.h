#pragma once
#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QStringList>

class OperateDB : public QObject
{
	Q_OBJECT
public:
	~OperateDB();
	static OperateDB* getInstance(); // 公用获取引用，实现单例模式

	bool UserRegister(const char* username, const char* password);
	bool UserLogin(const char* username, const char* password);
	QStringList GetUserOnline();
	int SearchUser(const char* name);//-1代表不存在，0代表存在但不在线，1代表存在且在线
	int AddUser(const char* friendName, const char* localName);//添加好友请求 -1表示错误 0表示离线 1表示在线 2表示好友存在
	bool AgreeAddUser(const char* friendName, const char* localName);//同意添加好友回复
	QStringList GetFriendList(const char* name);//获取好友列表
	bool DelFriend(const char* friendName, const char* localName);//删除好友


	void setOffline(const char* name);
private:
	explicit OperateDB(QObject* parent = Q_NULLPTR);
	void init();//初始化
	bool connectSql(const QString &dbName);

	QSqlDatabase m_db; // 连接数据库
	static OperateDB *instance;

};