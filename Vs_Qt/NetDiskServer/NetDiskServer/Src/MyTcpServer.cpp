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

void MyTcpServer::broadcastAddUserRes(QString friendName, QString localName)
{
	int re = OperateDB::getInstance()->AddUser(friendName.toStdString().c_str(), localName.toStdString().c_str());

	PDU* res_pdu = getPDU(0);//分配内存
	res_pdu->MsgType = ENUM_MSG_TYPE_ADD_USER_RESPONSE;//返回添加用户回应
	memset(res_pdu->caData, 0, 64);//清零
	//错误
	if (re == -1)
	{
		strcpy(res_pdu->caData, ADD_FRIEND_ERROR);
	}
	//下线
	else if (re == 0)
	{
		strcpy(res_pdu->caData, ADD_FRIEND_USER_OFFLINE);

	}
	//在线
	else if (re == 1)
	{
		strcpy(res_pdu->caData, ADD_FRIEND_USER_ONLINE);
	}
	//好友存在
	else if(re == 2)
	{
		strcpy(res_pdu->caData, ADD_FRIEND_IS_EXIST);
	}
	
	strcpy(res_pdu->caData + 32, localName.toStdString().c_str());//发送方的名字
	strcpy(res_pdu->caData + 48, friendName.toStdString().c_str());//接收方的名字

	//广播发送给每个客户端
	for (auto item : m_tcpSockets) 
	{
		item->write(reinterpret_cast<char *>(res_pdu), res_pdu->PDULen);
	}

	free(res_pdu);
	res_pdu = NULL;
}

void MyTcpServer::incomingConnection(qintptr socketDescriptor)
{
	qDebug() << "new connection";
	MyTcpSocket* tcpSkt = new MyTcpSocket(this);
	m_tcpSockets.push_back(tcpSkt);
	tcpSkt->setSocketDescriptor(socketDescriptor);//设置socket描述符

	connect(tcpSkt, &MyTcpSocket::disConnectedSign, this, &MyTcpServer::disConnectOneSocket);
	connect(tcpSkt, &MyTcpSocket::broadcastAddFriend, this, &MyTcpServer::broadcastAddUserRes);
}

MyTcpServer::MyTcpServer()
{

}
