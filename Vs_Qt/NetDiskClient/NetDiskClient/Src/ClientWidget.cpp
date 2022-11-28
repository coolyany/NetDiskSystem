#include "ClientWidget.h"
#include <QLabel>

ClientWidget::ClientWidget(QWidget * parent)
	: QWidget(parent)
{
	initLayout();
	initConnect();

	//m_statusBar->setStyleSheet("QStatusBar::item{border: 0px}");
	//m_statusBar->addPermanentWidget(new QLabel(QString("登录成功，账号：admin: %1").arg("admin")));//设置statusBar永久提示部件

}

ClientWidget & ClientWidget::getInstance()
{
	// TODO: 在此处插入 return 语句
	static ClientWidget instance;
	return instance;
}

void ClientWidget::initLayout()
{
	m_setWdt = new QListWidget(this);
	m_setWdt->addItem("好友");
	m_setWdt->addItem("图书");

	m_userWdt = new UserWidget();
	m_bookWdt = new BookWidget();
	m_stcWdt = new QStackedWidget;
	m_stcWdt->addWidget(m_userWdt);
	m_stcWdt->addWidget(m_bookWdt);


	QHBoxLayout* hLayout = new QHBoxLayout;
	hLayout->addWidget(m_setWdt);
	hLayout->addWidget(m_stcWdt);

	m_statusBar = new QStatusBar(this);
	hLayout->addWidget(m_statusBar);

	this->setLayout(hLayout);
	this->setMinimumHeight(500);
	this->setMinimumWidth(400);
}

void ClientWidget::initConnect()
{
	connect(m_setWdt, &QListWidget::currentRowChanged, m_stcWdt, &QStackedWidget::setCurrentIndex);
}

void ClientWidget::setWindowTitle(QString name)
{
	//this->setWindowTitle(name);
	m_userWdt->setLocalName(name);
}

void ClientWidget::setOnlineUser(PDU * pdu)
{
	if (!pdu)
	{
		return;
	}
	m_userWdt->setOnlineUserList(pdu);
}

void ClientWidget::setSearchUserResult(PDU * pdu)
{
	if (!pdu)
	{
		return;
	}
	m_userWdt->setSearchUserResult(pdu);
}
