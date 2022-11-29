#include "ClientWidget.h"
#include <QLabel>
#include <QPainter>
#include <QFile>
#include <QDesktopWidget>
#include <QApplication>
#include <QIcon>

#ifdef Q_OS_WIN
#include <qt_windows.h>
#include <Windowsx.h>
#endif

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
	hLayout->setMargin(0);

	/*m_statusBar = new QStatusBar(this);
	hLayout->addWidget(m_statusBar);*/

	//m_titleBar = new MyTitleBar(this);
	////m_titleBar->setTitleRoll();//设置跑马灯效果
	////m_titleBar->setTitleContent(QStringLiteral("前行中的小猪-前行之路还需前行"));
	//m_titleBar->setButtonType(MIN_MAX_BUTTON);
	//m_titleBar->setTitleWidth(this->width());

	this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
	// 设置窗口背景透明;
	this->setAttribute(Qt::WA_TranslucentBackground);
	m_titleBar = new MyTitleBar(this);
	m_titleBar->move(0, 0);
	m_titleBar->setTitleIcon(":/icon/icon/title.png");

	QVBoxLayout* vLayout = new QVBoxLayout;
	vLayout->addWidget(m_titleBar);
	vLayout->addLayout(hLayout);
	vLayout->setMargin(0);


	this->setLayout(vLayout);
	this->setMinimumHeight(500);
	this->setMinimumWidth(400);

	
	//this->installEventFilter(m_titleBar);

	m_nBorderWidth = 5;

	

	
}

void ClientWidget::paintEvent(QPaintEvent* event)
{
	//设置背景色;
	QPainter painter(this);
	QPainterPath pathBack;
	pathBack.setFillRule(Qt::WindingFill);
	pathBack.addRoundedRect(QRect(0, 0, this->width(), this->height()), 3, 3);
	painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
	painter.fillPath(pathBack, QBrush(QColor(238, 223, 204)));

	return QWidget::paintEvent(event);
}

void ClientWidget::loadStyleSheet(const QString & sheetName)
{
	QFile file(":/Resources/" + sheetName + ".css");
	file.open(QFile::ReadOnly);
	if (file.isOpen())
	{
		QString styleSheet = this->styleSheet();
		styleSheet += QLatin1String(file.readAll());
		this->setStyleSheet(styleSheet);
	}
}

void ClientWidget::initConnect()
{
	connect(m_setWdt, &QListWidget::currentRowChanged, m_stcWdt, &QStackedWidget::setCurrentIndex);

	connect(m_titleBar, SIGNAL(signalButtonMinClicked()), this, SLOT(onButtonMinClicked()));
	connect(m_titleBar, SIGNAL(signalButtonRestoreClicked()), this, SLOT(onButtonRestoreClicked()));
	connect(m_titleBar, SIGNAL(signalButtonMaxClicked()), this, SLOT(onButtonMaxClicked()));
	connect(m_titleBar, SIGNAL(signalButtonCloseClicked()), this, SLOT(onButtonCloseClicked()));
}

void ClientWidget::setWindowTitle(QString name)
{
	//this->setWindowTitle(name);
	m_userWdt->setLocalName(name);
	m_titleBar->setTitleContent(name);
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

//nativeEvent主要用于进程间通信-消息传递，使用这种方式后来实现窗体的缩放 [加上了这函数，窗口也能移动了]
bool ClientWidget::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
	Q_UNUSED(eventType)

		MSG *param = static_cast<MSG *>(message);

	switch (param->message)
	{
	case WM_NCHITTEST:
	{
		int nX = GET_X_LPARAM(param->lParam) - this->geometry().x();
		int nY = GET_Y_LPARAM(param->lParam) - this->geometry().y();

		// 如果鼠标位于子控件上，则不进行处理
		if (childAt(nX, nY) != nullptr)
			return QWidget::nativeEvent(eventType, message, result);

		*result = HTCAPTION;

		// 鼠标区域位于窗体边框，进行缩放
		if ((nX > 0) && (nX < m_nBorderWidth))
			*result = HTLEFT;

		if ((nX > this->width() - m_nBorderWidth) && (nX < this->width()))
			*result = HTRIGHT;

		if ((nY > 0) && (nY < m_nBorderWidth))
			*result = HTTOP;

		if ((nY > this->height() - m_nBorderWidth) && (nY < this->height()))
			*result = HTBOTTOM;

		if ((nX > 0) && (nX < m_nBorderWidth) && (nY > 0)
			&& (nY < m_nBorderWidth))
			*result = HTTOPLEFT;

		if ((nX > this->width() - m_nBorderWidth) && (nX < this->width())
			&& (nY > 0) && (nY < m_nBorderWidth))
			*result = HTTOPRIGHT;

		if ((nX > 0) && (nX < m_nBorderWidth)
			&& (nY > this->height() - m_nBorderWidth) && (nY < this->height()))
			*result = HTBOTTOMLEFT;

		if ((nX > this->width() - m_nBorderWidth) && (nX < this->width())
			&& (nY > this->height() - m_nBorderWidth) && (nY < this->height()))
			*result = HTBOTTOMRIGHT;

		return true;
	}
	}

	return QWidget::nativeEvent(eventType, message, result);
}

void ClientWidget::onButtonMinClicked()
{
	showMinimized();
}

void ClientWidget::onButtonRestoreClicked()
{
	QPoint windowPos;
	QSize windowSize;
	m_titleBar->getRestoreInfo(windowPos, windowSize);
	this->setGeometry(QRect(windowPos, windowSize));
}

void ClientWidget::onButtonMaxClicked()
{
	m_titleBar->saveRestoreInfo(this->pos(), QSize(this->width(), this->height()));
	QRect desktopRect = QApplication::desktop()->availableGeometry();
	QRect FactRect = QRect(desktopRect.x() - 3, desktopRect.y() - 3, desktopRect.width() + 6, desktopRect.height() + 6);
	setGeometry(FactRect);
}

void ClientWidget::onButtonCloseClicked()
{
	close();
}
