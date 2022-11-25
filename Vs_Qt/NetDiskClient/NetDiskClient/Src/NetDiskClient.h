﻿#pragma once

#include <QtWidgets/QWidget>
#include <QTcpSocket>
#include <QHostAddress>
#include "ui_NetDiskClient.h"
#include "QReadIni.h"
#include "myProtocol.h"
#include "ClientWidget.h"

class NetDiskClient : public QWidget
{
    Q_OBJECT

public:
    NetDiskClient(QWidget *parent = Q_NULLPTR);
	~NetDiskClient();

	void loadIniConfig();
	void initConnect();

	void handleRegisterRes(PDU* pdu);//处理服务端注册回应
	void handleLoginRes(PDU* pdu);//处理服务端登录回应
//槽函数
public:
	void buildConnected();
	void occurError(QAbstractSocket::SocketError socketError);
	void onReadyRead();

	void onRegisterButton();//注册
	void onLoginButton();//登录
private:
    Ui::NetDiskClientClass ui;

	/*QString m_ip;
	qint16 m_port;*/

	IniConfig m_iCfg;

	QTcpSocket* m_tcpSkt = nullptr;
	//ClientWidget* m_clienWdt = nullptr;
};
