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

void MyTcpServer::disConnectOneSocket(qintptr socketDescriptor)
{
	int i = 0;
	for (auto item : m_tcpSockets) {
		qDebug() << "客户端 " << item->socketDescriptor() << " 退出连接";
		if (item->socketDescriptor() == socketDescriptor)
		{
			m_tcpSockets.removeAt(i);
			item = nullptr;

			return;
		}
		qDebug() << "socket sizes :: " << m_tcpSockets.size();
		i++;
	}
}

void MyTcpServer::incomingConnection(qintptr socketDescriptor)
{
	qDebug() << "new connection";
	MyTcpSocket* tcpSkt = new MyTcpSocket(this);
	m_tcpSockets.push_back(tcpSkt);
	tcpSkt->setSocketDescriptor(socketDescriptor);//设置socket描述符

	connect(tcpSkt, &MyTcpSocket::disConnectedSign, this, &MyTcpServer::disConnectOneSocket);
}

MyTcpServer::MyTcpServer()
{

}
