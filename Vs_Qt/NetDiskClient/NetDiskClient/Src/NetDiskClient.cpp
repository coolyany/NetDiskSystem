#include "NetDiskClient.h"
#include <QDebug>
#include <QMessageBox>

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
	connect(m_tcpSkt, &QIODevice::readyRead, this, &NetDiskClient::onReadyRead);
	typedef void (QAbstractSocket::*QAbstractSocketErrorSignal)(QAbstractSocket::SocketError);
	connect(m_tcpSkt, static_cast<QAbstractSocketErrorSignal>(&QTcpSocket::error), this, &NetDiskClient::occurError);


	m_tcpSkt->connectToHost(QHostAddress::Any, m_iCfg.port);

	//connect(ui.pushButton, &QPushButton::clicked, [this]() {
	//	QString text = ui.lineEdit->text();
	//	if (text.isEmpty())
	//	{
	//		return;
	//	}
	//	ui.textEdit->setPlainText(text);

	//	PDU* pdu = getPDU(strlen(text.toStdString().c_str()) + 1);
	//	pdu->MsgType = 777;
	//	//printf("text :: %s\n", text.toStdString().c_str());
	//	//printf("text size :: %d\n", strlen(text.toStdString().c_str()) + 1);

	//	//memcpy((char*)&(pdu->caMsg), text.toLocal8Bit().data(), text.toLocal8Bit().size());//拷贝信息
	//	memcpy(reinterpret_cast<char*>(pdu->caMsg), text.toStdString().c_str(), strlen(text.toStdString().c_str()) + 1);//拷贝信息

	//	//printf("send msg :: %s\n", (char*)(pdu->caMsg));
	//	//printf("send msg reinterpret_cast :: %s\n", reinterpret_cast<char*>(pdu->caMsg));
	//	//printf("send msg size :: %d\n", sizeof((char*)(pdu->caMsg)));

	//	m_tcpSkt->write((char *)pdu, pdu->PDULen);//发送数据
	//	//清理内存
	//	free(pdu);
	//	pdu = NULL;
	//});
	connect(ui.pushButton_2, &QPushButton::clicked, this, &NetDiskClient::onRegisterButton);//注册
	connect(ui.pushButton, &QPushButton::clicked, this, &NetDiskClient::onLoginButton);//注册
}

NetDiskClient & NetDiskClient::getInstance()
{
	// TODO: 在此处插入 return 语句
	static NetDiskClient instance;
	return instance;
}

void NetDiskClient::handleRegisterRes(PDU* pdu)
{
	if (strcmp(pdu->caData, REGISTER_OK) == 0)
	{
		QMessageBox::information(this, "注册", REGISTER_OK);
	}
	else if(strcmp(pdu->caData, REGISTER_FAILED) == 0)
	{
		QMessageBox::warning(this, "注册", REGISTER_FAILED);

	}
}

void NetDiskClient::handleLoginRes(PDU * pdu)
{
	if (strcmp(pdu->caData, LOGIN_OK) == 0)
	{
		QMessageBox::information(this, "登录", LOGIN_OK);

		//跳转用户界面
		ClientWidget::getInstance().show();
		ClientWidget::getInstance().setWindowTitle(m_logname);

		//隐藏登录界面
		this->hide();
	}
	else if (strcmp(pdu->caData, LOGIN_FAILED) == 0)
	{
		QMessageBox::warning(this, "登录", LOGIN_FAILED); 
	}
}

void NetDiskClient::handleOnlineUserRes(PDU * pdu)
{
	ClientWidget::getInstance().setOnlineUser(pdu);
}

void NetDiskClient::handleSearchUserRes(PDU * pdu)
{
	ClientWidget::getInstance().setSearchUserResult(pdu);
}

void NetDiskClient::handleAddUserRes(PDU * pdu)
{
	char type[32] = { '\0' };
	char localName[16] = { '\0' };
	char friendName[16] = { '\0' };

	strcpy(type, pdu->caData);
	strcpy(localName, pdu->caData + 32);
	strcpy(friendName, pdu->caData + 48);


	qDebug() << "add friend res name :: " << friendName << "local name :: " << localName << " type :: " << type;
	//添加好友错误，发送方为本客户端时
	if ((strcmp(type, ADD_FRIEND_ERROR) == 0) && QString(localName) == m_logname)
	{
		QMessageBox::critical(this, "添加好友", "遇到错误，退出!!!");
	}
	//好友已存在，发送方为本客户端时
	else if ((strcmp(type, ADD_FRIEND_IS_EXIST) == 0) && QString(localName) == m_logname)
	{
		QMessageBox::warning(this, "添加好友", "好友已存在");
	}
	//在线，发送方为本客户端时
	else if ((strcmp(type, ADD_FRIEND_USER_ONLINE) == 0) && QString(localName) == m_logname)
	{
		QMessageBox::information(this, "添加好友", "发送申请成功,等待对方同意");
	}
	//在线，且为接收方时
	else if ((strcmp(type, ADD_FRIEND_USER_ONLINE) == 0) && QString(friendName) == m_logname)
	{
		int re = QMessageBox::information(this, "添加好友",
			QString("%1想添加你为好友，是否同意：").arg(localName),
			QMessageBox::Yes, QMessageBox::No);
		
		PDU* res_pdu = getPDU(0);//分配内存
		memset(res_pdu->caData, 0, 64);//清零
		
		if (re == QMessageBox::Yes)
		{
			qDebug() << "yes";
			res_pdu->MsgType = ENUM_MSG_TYPE_AGREE_ADD_USER_RESPONSE;//返回同意添加用户回应
			//strcpy(pdu->caData, friendName);
			//strcpy(pdu->caData + 32, localName);
			memcpy(res_pdu->caData, friendName, strlen(friendName) + 1);
			memcpy(res_pdu->caData + 32, localName, strlen(localName) + 1);


			m_tcpSkt->write(reinterpret_cast<char *>(res_pdu), res_pdu->PDULen);
			free(res_pdu);
			res_pdu = NULL;
		}
		if (re == QMessageBox::No)
		{
			qDebug() << "no";
		}
		
	}
	//离线，发送方为本客户端时
	else if ((strcmp(type, ADD_FRIEND_USER_OFFLINE) == 0) && QString(localName) == m_logname)
	{
		QMessageBox::information(this, "添加好友", "用户处于离线状态，申请已发送");
	}

}

void NetDiskClient::handleShowFriendList(PDU * pdu)
{
	ClientWidget::getInstance().setFriendList(pdu);
}

void NetDiskClient::handleDelFriendRes(PDU * pdu)
{
	ClientWidget::getInstance().setDelFriendList(pdu);
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

void NetDiskClient::onReadyRead()
{
	printf("start read...\n");
	
	uint PDULen = 0;
	m_tcpSkt->read((char*)&PDULen, sizeof(uint));
	//获取发送信息的大小
	uint MSGLen = PDULen - sizeof(PDU);
	PDU* pdu = getPDU(MSGLen);//分配内存
	m_tcpSkt->read((char*)pdu + sizeof(uint), PDULen - sizeof(uint));

	switch (pdu->MsgType) {
	case ENUM_MSG_TYPE_REGIST_RESPONSE:
		handleRegisterRes(pdu);
		break;
	case ENUM_MSG_TYPE_LOGIN_RESPONSE:
		handleLoginRes(pdu);
		break;
	case ENUM_MSG_TYPE_USER_ONLINE_RESPONSE:
		handleOnlineUserRes(pdu);
		break;
	case ENUM_MSG_TYPE_SEARCH_USER_RESPONSE:
		handleSearchUserRes(pdu);
		break;
	case ENUM_MSG_TYPE_ADD_USER_RESPONSE:
		handleAddUserRes(pdu);
		break;
	case ENUM_MSG_TYPE_REFRESH_FRIEND_LIST_RESPONSE:
		handleShowFriendList(pdu);
		break;
	case ENUM_MSG_TYPE_DELETE_FRIEND_RESPONSE:
		handleDelFriendRes(pdu);
		break;
	default:
		break;
	}


	//clear
	free(pdu);
	pdu = NULL;
}

void NetDiskClient::onRegisterButton()
{
	//获取输入数据
	QString username = ui.lineEdit->text();
	QString password = ui.lineEdit_2->text();
	if (username.isEmpty() || password.isEmpty())
	{
		QMessageBox::critical(this, "注册", "注册失败，用户名或密码为空");
	}
	else
	{
		PDU* pdu = getPDU(0);
		pdu->MsgType = ENUM_MSG_TYPE_REGIST_REQUEST;
		memcpy(pdu->caData, username.toStdString().c_str(), 32);//拷贝信息
		memcpy(pdu->caData + 32, password.toStdString().c_str(), 32);//拷贝信息

		m_tcpSkt->write((char *)pdu, pdu->PDULen);//发送数据
		//清理内存
		free(pdu);
		pdu = NULL;
	}
}

void NetDiskClient::onLoginButton()
{
	//获取输入数据
	QString username = ui.lineEdit->text();
	QString password = ui.lineEdit_2->text();
	if (username.isEmpty() || password.isEmpty())
	{
		QMessageBox::critical(this, "登录", "登录失败，用户名或密码错误，或改用户处于在线状态");
	}
	else
	{
		m_logname = username;
		PDU* pdu = getPDU(0);
		pdu->MsgType = ENUM_MSG_TYPE_LOGIN_REQUEST;
		memcpy(pdu->caData, username.toStdString().c_str(), username.toStdString().size());//拷贝信息
		memcpy(pdu->caData + 32, password.toStdString().c_str(), password.toStdString().size());//拷贝信息

		m_tcpSkt->write((char *)pdu, pdu->PDULen);//发送数据
		//清理内存
		free(pdu);
		pdu = NULL;
	}
}
