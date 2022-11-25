#include "ClientWidget.h"

ClientWidget::ClientWidget(QWidget * parent)
	: QWidget(parent)
{
	initLayout();
	initConnect();


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


	this->setLayout(hLayout);
}

void ClientWidget::initConnect()
{
	connect(m_setWdt, &QListWidget::currentRowChanged, m_stcWdt, &QStackedWidget::setCurrentIndex);
}
