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

	void initLayout();
	void initConnect();
private:
	QListWidget *m_setWdt = nullptr;

	UserWidget* m_userWdt = nullptr;
	BookWidget* m_bookWdt = nullptr;
	QStackedWidget* m_stcWdt = nullptr;
};