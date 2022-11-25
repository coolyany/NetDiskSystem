#include "NetDiskServer.h"
#include <QDebug>

NetDiskServer::NetDiskServer(QWidget *parent)
    : QWidget(parent)
{
    //ui.setupUi(this);
	loadIniConfig();
	initConnect();
}

void NetDiskServer::loadIniConfig()
{
	m_iCfg = QReadIni::getInstance()->getIniConfig();

	qDebug() << "ip :: " << m_iCfg.ip << " port :: " << m_iCfg.port;
}

void NetDiskServer::initConnect()
{
	//监听
	MyTcpServer* tcpsvr = MyTcpServer::getInstance();
	tcpsvr->listen(QHostAddress::Any, m_iCfg.port);
	if (tcpsvr->isListening())
	{
		qDebug() << "server is listening ...";
	}
}
