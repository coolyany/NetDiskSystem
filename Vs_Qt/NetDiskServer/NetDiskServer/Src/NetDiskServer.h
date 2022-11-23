#pragma once

#include <QtWidgets/QWidget>
#include <QHostAddress>

#include "ui_NetDiskServer.h"
#include "QReadIni.h"
#include "MyTcpServer.h"

class NetDiskServer : public QWidget
{
    Q_OBJECT

public:
    NetDiskServer(QWidget *parent = Q_NULLPTR);

	void loadIniConfig();
	void initConnect();

private:
    Ui::NetDiskServerClass ui;

	IniConfig m_iCfg;
};
