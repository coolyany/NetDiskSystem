#pragma once
#include <QTcpSocket>
#include <QVariant>
#include "myProtocol.h"
#include "OperateDB.h"

class MyTcpSocket : public QTcpSocket
{
	Q_OBJECT;
public:
	MyTcpSocket(QObject* parent = nullptr);

//成员函数
public:
	void handleRegisterReq(PDU* pdu);//处理注册请求
	void handleLoginReq(PDU* pdu);//处理登录请求
	void handleUserOnlineReq();//处理用户在线请求
	void handleSearchUserReq(PDU* pdu);//处理搜索用户请求
	void handleAddUserReq(PDU* pdu);//处理添加用户的请求
	void handleAgreeAddUserRes(PDU* pdu);//处理同意添加好友回复


	inline QString getClientName() { return m_username; }
	bool isLocalName(QString name);
//槽函数
public:
	void ReadMsg();

signals:
	void disConnectedSign(qintptr socketDescriptor);
	void broadcastAddFriend(QString, QString);//广播添加好友通知
private:
	QString m_username;
};