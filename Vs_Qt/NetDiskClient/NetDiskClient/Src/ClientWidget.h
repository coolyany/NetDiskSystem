#pragma once

#include <QWidget>
#include <QListWidget>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QStatusBar>

#include "UserWidget.h"
#include "BookWidget.h"
#include "MyTitleBar.h"

class ClientWidget : public QWidget
{
	Q_OBJECT
public:
	ClientWidget(QWidget* parent = Q_NULLPTR);

	static ClientWidget& getInstance();

	void initLayout();
	void initConnect();
	void setWindowTitle(QString name);

public:
	void setOnlineUser(PDU* pdu);
	void setSearchUserResult(PDU* pdu);
	void setFriendList(PDU* pdu);
	void setDelFriendList(PDU* pdu);

	void loadStyleSheet(const QString &sheetName);

public slots:
	void onButtonMinClicked();
	void onButtonRestoreClicked();
	void onButtonMaxClicked();
	void onButtonCloseClicked();
protected:
	void paintEvent(QPaintEvent *event) override;
	bool nativeEvent(const QByteArray &eventType, void *message, long *result) override;
private:

	QListWidget *m_setWdt = nullptr;

	UserWidget* m_userWdt = nullptr;
	BookWidget* m_bookWdt = nullptr;
	QStackedWidget* m_stcWdt = nullptr;
	QStatusBar* m_statusBar = nullptr;
	MyTitleBar* m_titleBar = nullptr;

	int m_nBorderWidth;
};