#include "MyTcpSocket.h"
#include <QDebug>
MyTcpSocket::MyTcpSocket(QObject * parent)
	:QTcpSocket(parent)
{
	connect(this, &MyTcpSocket::readyRead, this, &MyTcpSocket::ReadMsg);
}

void MyTcpSocket::ReadMsg()
{
	qDebug() << "read size :: " << this->bytesAvailable();
	//先读取自定义的信息包整个大小,4个字节
	uint PDULen = 0;
	this->read((char*)&PDULen, sizeof(uint));
	//获取发送信息的大小
	uint MSGLen = PDULen - sizeof(PDU);
	PDU* pdu = getPDU(MSGLen);//分配内存
	this->read((char*)pdu + sizeof(uint), PDULen - sizeof(uint));

	//qDebug() << "type :: " << pdu->MsgType << " msg :: " << (char *)(pdu->caMsg);

	printf("recv type :: %d, msg :: %s\n", pdu->MsgType, reinterpret_cast<const char*>(pdu->caMsg));

}
