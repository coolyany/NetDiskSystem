#include "MyTcpServer.h"
#include <QDebug>

MyTcpServer* MyTcpServer::instance = nullptr;

MyTcpServer *MyTcpServer::getInstance()
{
	// TODO: 在此处插入 return 语句
	if (!instance)
	{
		instance = new MyTcpServer();
	}
	return instance;
}

void MyTcpServer::incomingConnection(qintptr socketDescriptor)
{
	qDebug() << "new connection";
	MyTcpSocket* tcpSkt = new MyTcpSocket(this);
	m_tcpSockets.push_back(tcpSkt);
	tcpSkt->setSocketDescriptor(socketDescriptor);//设置socket描述符

}

MyTcpServer::MyTcpServer()
{

}
