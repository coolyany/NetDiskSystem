#include "MyTcpSocket.h"
#include <QDebug>

MyTcpSocket::MyTcpSocket(QObject * parent)
	:QTcpSocket(parent)
{
	/*m_db = MySql::getInstance();
	m_db->init();*/

	connect(this, &MyTcpSocket::readyRead, this, &MyTcpSocket::ReadMsg);
	connect(this, &MyTcpSocket::disconnected, [this]() {
		qDebug() << this->socketDescriptor() << "断开连接";
		OperateDB::getInstance()->setOffline(m_username.toStdString().c_str());
		emit disConnectedSign(this->socketDescriptor());
	});
}

void MyTcpSocket::handleRegisterReq(PDU* pdu)
{
	char name[32] = { '\0' };
	char pwd[32] = { '\0' };
	strncpy(name, pdu->caData, 32);
	strncpy(pwd, pdu->caData + 32, 32);

	qDebug() << "name :: " << name << " pwd :: " << pwd;

	bool re = OperateDB::getInstance()->UserRegister(name, pwd);

	PDU* res_pdu = getPDU(0);//分配内存
	res_pdu->MsgType = ENUM_MSG_TYPE_REGIST_RESPONSE;//返回注册回应
	//数据库操作成功
	if (re)
	{
		//memcpy(res_pdu->caData, REGISTER_OK, 64);
		strcpy(res_pdu->caData, REGISTER_OK);
	}
	//失败
	else
	{
		//memcpy(res_pdu->caData, REGISTER_FAILED, 64);
		strcpy(res_pdu->caData, REGISTER_FAILED);

	}
	this->write(reinterpret_cast<char *>(res_pdu), res_pdu->PDULen);


	free(res_pdu);
	res_pdu = NULL;
}

void MyTcpSocket::handleLoginReq(PDU * pdu)
{
	char name[32] = { '\0' };
	char pwd[32] = { '\0' };
	strncpy(name, pdu->caData, 32);
	strncpy(pwd, pdu->caData + 32, 32);

	qDebug() << "name :: " << name << " pwd :: " << pwd;
	m_username = name;
	//bool re = OperateDB::getInstance()->UserRegister(name, pwd);
	bool re = OperateDB::getInstance()->UserLogin(name, pwd);


	PDU* res_pdu = getPDU(0);//分配内存
	res_pdu->MsgType = ENUM_MSG_TYPE_LOGIN_RESPONSE;//返回登录回应
	//数据库操作成功
	if (re)
	{
		strcpy(res_pdu->caData, LOGIN_OK);
	}
	//失败
	else
	{
		strcpy(res_pdu->caData, LOGIN_FAILED);

	}
	this->write(reinterpret_cast<char *>(res_pdu), res_pdu->PDULen);


	free(res_pdu);
	res_pdu = NULL;
}

void MyTcpSocket::handleUserOnlineReq()
{
	QStringList inline_users = OperateDB::getInstance()->GetUserOnline();

	PDU* res_pdu = getPDU(inline_users.size() * 32);//分配信息的内存，每个用户占32个字节
	res_pdu->MsgType = ENUM_MSG_TYPE_USER_ONLINE_RESPONSE;//返回用户在线回应

	//回应数据
	for (size_t i = 0; i < inline_users.size(); i++)
	{
		//转换用户名->const char*
		//const char* name = inline_users[i].toStdString().c_str();
		QString name = inline_users.at(i);
		printf("length :: %d\n", name.size() + 1);
		//拷贝,内存地址每次移32位
		memcpy((char*)(res_pdu->caMsg) + i*32, name.toStdString().c_str(), name.size() + 1);
	}

	this->write(reinterpret_cast<char *>(res_pdu), res_pdu->PDULen);
	free(res_pdu);
	res_pdu = NULL;
}

void MyTcpSocket::handleSearchUserReq(PDU* pdu)
{
	char name[64] = { '\0' };
	strncpy(name, pdu->caData, 64);
	qDebug() << "search username is :: " << name;

	//数据库查询
	int re = OperateDB::getInstance()->SearchUser(name);

	PDU* res_pdu = getPDU(0);//分配内存
	res_pdu->MsgType = ENUM_MSG_TYPE_SEARCH_USER_RESPONSE;//返回登录回应
	memset(res_pdu->caData, 0, 64);//清零
	//返回数据
	//不存在
	if (re == -1)
	{
		strcpy(res_pdu->caData, SEARCH_NO_RESULT);
	}
	//离线
	else if (re == 0)
	{
		strcpy(res_pdu->caData, SEARCH_OK_OFFLINE);
	}
	//在线
	else if (re == 1)
	{
		strcpy(res_pdu->caData, SEARCH_OK_ONLINE);
	}
	
	this->write(reinterpret_cast<char *>(res_pdu), res_pdu->PDULen);


	free(res_pdu);
	res_pdu = NULL;
}

void MyTcpSocket::handleAddUserReq(PDU * pdu)
{
	//char friendName[32] = { '\0' };
	//char localName[32] = { '\0' };

	//strncpy(friendName, pdu->caData, 32);
	//strncpy(localName, pdu->caData + 32, 32);

	//qDebug() << "add friend name is :: " << friendName;
	//qDebug() << "add local name is :: " << localName;

	//int re = OperateDB::getInstance()->AddUser(friendName, localName);

	//PDU* res_pdu = getPDU(0);//分配内存
	//res_pdu->MsgType = ENUM_MSG_TYPE_ADD_USER_RESPONSE;//返回添加用户回应
	//memset(res_pdu->caData, 0, 64);//清零
	////错误
	//if (re == -1)
	//{
	//	strcpy(res_pdu->caData, ADD_FRIEND_ERROR);
	//}
	////下线
	//else if (re == 0)
	//{
	//	strcpy(res_pdu->caData, ADD_FRIEND_USER_OFFLINE);

	//}
	////在线
	//else if (re == 1)
	//{
	//	strcpy(res_pdu->caData, ADD_FRIEND_USER_ONLINE);

	//}
	////好友存在
	//else if(re == 2)
	//{
	//	strcpy(res_pdu->caData, ADD_FRIEND_IS_EXIST);
	//}
	//strcpy(res_pdu->caData + 32, friendName);//需要添加的好友名
	//this->write(reinterpret_cast<char *>(res_pdu), res_pdu->PDULen);

	//free(res_pdu);
	//res_pdu = NULL;


	char friendName[32] = { '\0' };
	char localName[32] = { '\0' };

	strncpy(friendName, pdu->caData, 32);
	strncpy(localName, pdu->caData + 32, 32);

	emit broadcastAddFriend(QString(friendName), QString(localName));
}

void MyTcpSocket::handleAgreeAddUserRes(PDU* pdu)
{
	char localName[32] = { '\0' };
	char friendName[32] = { '\0' };

	strncpy(friendName, pdu->caData, 32);
	strncpy(localName, pdu->caData + 32, 32);

	qDebug() << "search username is :: " << friendName;
	qDebug() << "search username is :: " << localName;


	int re = OperateDB::getInstance()->AgreeAddUser(friendName, localName);
	if (re)
	{
		qDebug() << "agree add friend successful";
	}
	else
	{
		qDebug() << "agree add friend failed";

	}
}

bool MyTcpSocket::isLocalName(QString name)
{
	return name == m_username ? true : false;
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

	//printf("recv type :: %d, msg :: %s\n", pdu->MsgType, reinterpret_cast<const char*>(pdu->caMsg));

	switch (pdu->MsgType) {
	case ENUM_MSG_TYPE_REGIST_REQUEST:
		handleRegisterReq(pdu);
		break;
	case ENUM_MSG_TYPE_LOGIN_REQUEST:
		handleLoginReq(pdu);
		break;
	case ENUM_MSG_TYPE_USER_ONLINE_RESPONSE:
		handleUserOnlineReq();
		break;
	case ENUM_MSG_TYPE_SEARCH_USER_REQUEST:
		handleSearchUserReq(pdu);
		break;
	case ENUM_MSG_TYPE_ADD_USER_REQUEST:
		handleAddUserReq(pdu);
		break;
	case ENUM_MSG_TYPE_AGREE_ADD_USER_RESPONSE:
		handleAgreeAddUserRes(pdu);
		break;
	default:
		break;
	}


	//clear
	free(pdu);
	pdu = NULL;
}
