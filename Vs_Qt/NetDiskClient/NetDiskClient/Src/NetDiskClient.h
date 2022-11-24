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

	void handleRegisterRes(PDU* pdu);//处理服务端回应
//槽函数
public:
	void buildConnected();
	void occurError(QAbstractSocket::SocketError socketError);
	void onReadyRead();

	void onRegisterButton();//注册
private:
    Ui::NetDiskClientClass ui;

	/*QString m_ip;
	qint16 m_port;*/

	IniConfig m_iCfg;

	QTcpSocket* m_tcpSkt = nullptr;
};
