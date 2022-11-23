#pragma once
#include <QTcpServer>
#include <QList>

#include "MyTcpSocket.h"
//单例模式
class MyTcpServer : public QTcpServer
{
	Q_OBJECT;
public:
	static MyTcpServer* getInstance();

	void disConnectOneSocket(qintptr socketDescriptor);
protected:
	void incomingConnection(qintptr socketDescriptor) override;

private:
	MyTcpServer();

	static MyTcpServer* instance;

	QList<MyTcpSocket*> m_tcpSockets;
};