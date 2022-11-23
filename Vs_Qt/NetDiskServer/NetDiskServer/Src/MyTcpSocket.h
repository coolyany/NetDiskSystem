#pragma once
#include <QTcpSocket>
#include "myProtocol.h"

class MyTcpSocket : public QTcpSocket
{
	Q_OBJECT;
public:
	MyTcpSocket(QObject* parent = nullptr);

//槽函数
public:
	void ReadMsg();
private:

};