#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_NetDiskSystem.h"
#include "MySql.h"

class NetDiskSystem : public QMainWindow
{
    Q_OBJECT

public:
    NetDiskSystem(QWidget *parent = Q_NULLPTR);

private:
    Ui::NetDiskSystemClass ui;
};
