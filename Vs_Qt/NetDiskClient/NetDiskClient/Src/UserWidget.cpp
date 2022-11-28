#include "UserWidget.h"
#include "NetDiskClient.h"
#include <QInputDialog>
#include <QDebug>
#include <QMessageBox>

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

	//安装事件过滤器
	m_userList->installEventFilter(this);
}

void UserWidget::initConnect()
{
	connect(m_showOnlineListPB, &QPushButton::clicked, this, &UserWidget::onShowOnlineList);
	connect(m_findUserPB, &QPushButton::clicked, this, &UserWidget::onShowSearchUser);
	//connect(m_userList, &QListWidget::itemPressed, this, &UserWidget::onClickedUsersListItem);
	connect(m_addFriendPB, &QPushButton::clicked, this, &UserWidget::onClickAddFrien);
}

void UserWidget::clearUserList()
{
	m_userList->clear();
}

void UserWidget::setOnlineUserList(PDU * pdu)
{
	//清空用户显示列表
	clearUserList();
	//获取user个数
	uint userNum = pdu->MsgLen / 32;
	char usrDst[32] = { '\0' };
	for (uint i = 0; i < userNum; i++)
	{
		memcpy(usrDst, (char*)(pdu->caMsg) + i * 32, 32);
		this->m_userList->addItem(usrDst);
	}
}

void UserWidget::setSearchUserResult(PDU * pdu)
{
	//清空用户显示列表
	clearUserList();

	//搜索成功且在线
	if (strcmp(pdu->caData, SEARCH_OK_ONLINE) == 0)
	{
		m_userList->addItem(QString("%1,在线").arg(m_inputName));
	}
	//搜索成功但没在线
	else if (strcmp(pdu->caData, SEARCH_OK_OFFLINE) == 0)
	{
		m_userList->addItem(QString("%1,下线").arg(m_inputName));

	}
	//搜索无结果
	else if (strcmp(pdu->caData, SEARCH_NO_RESULT) == 0)
	{
		QMessageBox::warning(this, "查询用户", "无结果");
	}
}

void UserWidget::onShowOnlineList()
{
	PDU* pdu = getPDU(0);
	pdu->MsgType = ENUM_MSG_TYPE_USER_ONLINE_RESPONSE;//在线请求标志
	memset(pdu->caData, 0, 64);

	NetDiskClient::getInstance().getTcpSocket()->write((char *)pdu, pdu->PDULen);//发送数据
	//清理内存
	free(pdu);
	pdu = NULL;
}

void UserWidget::onShowSearchUser()
{
	QString name = QInputDialog::getText(this, "搜索", "用户名:");
	if (!name.isEmpty())
	{
		qDebug() << "input name :: " << name;
		m_inputName = name;
		PDU* pdu = getPDU(0);
		pdu->MsgType = ENUM_MSG_TYPE_SEARCH_USER_REQUEST;//搜索用户请求
		memset(pdu->caData, 0, 64);
		strcpy(pdu->caData, name.toStdString().c_str());

		NetDiskClient::getInstance().getTcpSocket()->write((char *)pdu, pdu->PDULen);//发送数据
		//清理内存
		free(pdu);
		pdu = NULL;
	}
	
}

void UserWidget::onClickedUsersListItem(QListWidgetItem * item)
{
	qDebug() << "item is clicked ... ";
	m_curUserItem = item;
}

void UserWidget::onClickAddFrien()
{
	if (m_curUserItem->isSelected())
	{
		qDebug() << "add friend";
	}
}

bool UserWidget::eventFilter(QObject * watched, QEvent * event)
{
	if (watched == m_userList)
	{
		qDebug() << " eventFilter ...";
		if (QEvent::MouseButtonPress == event->type())
		{
			qDebug() << "Clicked ...";
			
			m_curUserItem = m_userList->itemAt(QCursor::pos());
			if (m_curUserItem)
			{
				qDebug() << "Clicked Not Null";
			}
			else
			{
				qDebug() << "Clicked NULL";
			}
		}
	}
	return QWidget::eventFilter(watched, event);
}
