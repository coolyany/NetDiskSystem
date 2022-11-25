#include "UserWidget.h"
#include "NetDiskClient.h"


UserWidget::UserWidget(QWidget * parent)
{
	initLayout();
	initConnect();
}

void UserWidget::initLayout()
{
	m_showMsgTE = new QTextEdit();
	m_sendMsgLE = new QLineEdit();
	m_userList = new QListWidget();
	m_delFriendPB = new QPushButton("删除好友");
	m_addFriendPB = new QPushButton("添加好友");
	m_showFriendListPB = new QPushButton("显示好友列表");
	m_showOnlineListPB = new QPushButton("显示在线列表");
	m_findUserPB = new QPushButton("查找用户");
	m_privateChatPB = new QPushButton("私聊");
	m_sendMsgPB = new QPushButton("发送");

	QVBoxLayout* rightVLayout = new QVBoxLayout;
	rightVLayout->addWidget(m_delFriendPB);
	rightVLayout->addWidget(m_addFriendPB);
	rightVLayout->addWidget(m_showFriendListPB);
	rightVLayout->addWidget(m_showOnlineListPB);
	rightVLayout->addWidget(m_findUserPB);
	rightVLayout->addWidget(m_privateChatPB);


	QHBoxLayout* leftHLayout = new QHBoxLayout;
	leftHLayout->addWidget(m_showMsgTE);
	leftHLayout->addWidget(m_userList);
	leftHLayout->addLayout(rightVLayout);

	QHBoxLayout* bottomHLayout = new QHBoxLayout;
	//bottomHLayout->addLayout(leftHLayout);
	bottomHLayout->addWidget(m_sendMsgLE);
	bottomHLayout->addWidget(m_sendMsgPB);

	QVBoxLayout *wdtLayout = new QVBoxLayout;
	wdtLayout->addLayout(leftHLayout);
	wdtLayout->addLayout(bottomHLayout);


	this->setLayout(wdtLayout);


}

void UserWidget::initConnect()
{
	connect(m_showOnlineListPB, &QPushButton::clicked, this, &UserWidget::onShowOnlineList);
}

void UserWidget::setOnlineUserList(PDU * pdu)
{
	//获取user个数
	uint userNum = pdu->MsgLen / 32;
	char usrDst[32] = { '\0' };
	for (uint i = 0; i < userNum; i++)
	{
		memcpy(usrDst, (char*)(pdu->caMsg) + i * 32, 32);
		this->m_userList->addItem(usrDst);
	}
}

void UserWidget::onShowOnlineList()
{
	PDU* pdu = getPDU(0);
	pdu->MsgType = ENUM_MSG_TYPE_USER_ONLINE_RESPONSE;//在线请求标志
	//memset(pdu->caData, 0, 64);

	NetDiskClient::getInstance().getTcpSocket()->write((char *)pdu, pdu->PDULen);//发送数据
	//清理内存
	free(pdu);
	pdu = NULL;
}
