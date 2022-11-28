#pragma once

#include <QWidget>
#include <QListWidget>
#include <QHBoxLayout>
#include <QStackedWidget>

#include "UserWidget.h"
#include "BookWidget.h"

class ClientWidget : public QWidget
{
public:
	ClientWidget(QWidget* parent = Q_NULLPTR);

	static ClientWidget& getInstance();

	void initLayout();
	void initConnect();
	void setWindowTittle(QString name);

public:
	void setOnlineUser(PDU* pdu);
	void setSearchUserResult(PDU* pdu);
private:


	QListWidget *m_setWdt = nullptr;

	UserWidget* m_userWdt = nullptr;
	BookWidget* m_bookWdt = nullptr;
	QStackedWidget* m_stcWdt = nullptr;
};