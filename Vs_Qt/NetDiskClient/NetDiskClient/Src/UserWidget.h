#pragma once
#include <QWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QListWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "myProtocol.h"

class UserWidget : public QWidget
{
public:
	UserWidget(QWidget* parent = Q_NULLPTR);

	void initLayout();//界面布局
	void initConnect();
	void clearUserList();//清空显示的用户列表

	void setOnlineUserList(PDU* pdu);
	void setSearchUserResult(PDU* pdu);
public:
	void onShowOnlineList();
	void onShowSearchUser();
	void onClickedUsersListItem(QListWidgetItem *item);
	void onClickAddFrien();//添加好友

protected:
	bool eventFilter(QObject* watched, QEvent* event) override;
private:
	QTextEdit* m_showMsgTE = nullptr;
	QLineEdit* m_sendMsgLE = nullptr;
	QListWidget* m_userList = nullptr;
	
	QPushButton* m_delFriendPB = nullptr;
	QPushButton* m_addFriendPB = nullptr;
	QPushButton* m_showFriendListPB = nullptr;
	QPushButton* m_showOnlineListPB = nullptr;
	QPushButton* m_findUserPB = nullptr;
	QPushButton* m_privateChatPB = nullptr;
	QPushButton* m_sendMsgPB = nullptr;

	QListWidgetItem* m_curUserItem = nullptr;

	QString m_inputName;
};
