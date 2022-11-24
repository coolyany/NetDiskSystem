#pragma once
#include <QTcpSocket>
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

//槽函数
public:
	void ReadMsg();

signals:
	void disConnectedSign(qintptr socketDescriptor);
private:
};