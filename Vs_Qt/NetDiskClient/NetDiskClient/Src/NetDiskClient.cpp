#include "NetDiskClient.h"
#include <QDebug>
NetDiskClient::NetDiskClient(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

	loadIniConfig();
	initConnect();
}

NetDiskClient::~NetDiskClient()
{
	if (m_tcpSkt)
	{
		delete m_tcpSkt;
		m_tcpSkt = nullptr;
	}
}

void NetDiskClient::loadIniConfig()
{
	//加载配置文件
	m_iCfg = QReadIni::getInstance()->getIniConfig();
}

void NetDiskClient::initConnect()
{
	m_tcpSkt = new QTcpSocket(this);
	connect(m_tcpSkt, &QTcpSocket::connected, this, &NetDiskClient::buildConnected);
	//connect(m_tcpSkt, &QIODevice::readyRead, this, &NetDiskClient::readyRead);
	typedef void (QAbstractSocket::*QAbstractSocketErrorSignal)(QAbstractSocket::SocketError);
	connect(m_tcpSkt, static_cast<QAbstractSocketErrorSignal>(&QTcpSocket::error), this, &NetDiskClient::occurError);

	m_tcpSkt->connectToHost(QHostAddress::Any, m_iCfg.port);

	connect(ui.pushButton, &QPushButton::clicked, [this]() {
		QString text = ui.lineEdit->text();
		if (text.isEmpty())
		{
			return;
		}

		PDU* pdu = getPDU(strlen(text.toStdString().c_str()) + 1);
		pdu->MsgType = 777;
		//printf("text :: %s\n", text.toStdString().c_str());
		//printf("text size :: %d\n", strlen(text.toStdString().c_str()) + 1);

		//memcpy((char*)&(pdu->caMsg), text.toLocal8Bit().data(), text.toLocal8Bit().size());//拷贝信息
		memcpy(reinterpret_cast<char*>(pdu->caMsg), text.toStdString().c_str(), strlen(text.toStdString().c_str()) + 1);//拷贝信息

		//printf("send msg :: %s\n", (char*)(pdu->caMsg));
		//printf("send msg reinterpret_cast :: %s\n", reinterpret_cast<char*>(pdu->caMsg));
		//printf("send msg size :: %d\n", sizeof((char*)(pdu->caMsg)));

		m_tcpSkt->write((char *)pdu, pdu->PDULen);//发送数据
		//清理内存
		free(pdu);
		pdu = NULL;
	});
}

void NetDiskClient::buildConnected()
{
	printf("connect successful\n");
}

void NetDiskClient::occurError(QAbstractSocket::SocketError socketError)
{
	//printf("connect successful\n");
	qDebug() << "客户端错误信息：" << socketError;
}

void NetDiskClient::readyRead()
{
	printf("start read...\n");

}
