#pragma once

#include <QtWidgets/QWidget>
#include <QTcpSocket>
#include <QHostAddress>
#include "ui_NetDiskClient.h"
#include "QReadIni.h"
#include "myProtocol.h"

class NetDiskClient : public QWidget
{
    Q_OBJECT

public:
    NetDiskClient(QWidget *parent = Q_NULLPTR);
	~NetDiskClient();

	void loadIniConfig();
	void initConnect();
//槽函数
public:
	void buildConnected();
	void occurError(QAbstractSocket::SocketError socketError);
	void readyRead();
private:
    Ui::NetDiskClientClass ui;

	/*QString m_ip;
	qint16 m_port;*/

	IniConfig m_iCfg;

	QTcpSocket* m_tcpSkt = nullptr;
};
