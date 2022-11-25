#include "UserWidget.h"

UserWidget::UserWidget(QWidget * parent)
{
	interfaceLayout();
}

void UserWidget::interfaceLayout()
{
	m_showMsgTE = new QTextEdit();
	m_sendMsgLE = new QLineEdit();
	m_friendList = new QListWidget();
	m_delFriendPB = new QPushButton("删除好友");
	m_addFriendPB = new QPushButton("添加好友");
	m_updFriendListPB = new QPushButton("更新好友列表");
	m_showFriendListPB = new QPushButton("显示好友列表");
	m_findUserPB = new QPushButton("查找用户");
	m_privateChatPB = new QPushButton("私聊");
	m_sendMsgPB = new QPushButton("发送");

	QVBoxLayout* rightVLayout = new QVBoxLayout;
	rightVLayout->addWidget(m_delFriendPB);
	rightVLayout->addWidget(m_addFriendPB);
	rightVLayout->addWidget(m_updFriendListPB);
	rightVLayout->addWidget(m_showFriendListPB);
	rightVLayout->addWidget(m_findUserPB);
	rightVLayout->addWidget(m_privateChatPB);


	QHBoxLayout* leftHLayout = new QHBoxLayout;
	leftHLayout->addWidget(m_showMsgTE);
	leftHLayout->addWidget(m_friendList);
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
