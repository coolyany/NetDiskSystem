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
		if (item->socketDescriptor() == socketDescriptor)
		{
			qDebug() << "客户端 " << item->getClientName() << " 退出连接";

			m_tcpSockets.removeAt(i);
			item = nullptr;
			break;
		}
		i++;
	}
	qDebug() << "logouted socket number :: " << m_tcpSockets.size();

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
