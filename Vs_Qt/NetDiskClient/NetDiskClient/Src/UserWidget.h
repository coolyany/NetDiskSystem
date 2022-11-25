#pragma once
#include <QWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QListWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>

class UserWidget : public QWidget
{
public:
	UserWidget(QWidget* parent = Q_NULLPTR);

	void interfaceLayout();//界面布局

private:
	QTextEdit* m_showMsgTE = nullptr;
	QLineEdit* m_sendMsgLE = nullptr;
	QListWidget* m_friendList = nullptr;
	
	QPushButton* m_delFriendPB = nullptr;
	QPushButton* m_addFriendPB = nullptr;
	QPushButton* m_updFriendListPB = nullptr;
	QPushButton* m_showFriendListPB = nullptr;
	QPushButton* m_findUserPB = nullptr;
	QPushButton* m_privateChatPB = nullptr;
	QPushButton* m_sendMsgPB = nullptr;

};
