#include "NetDiskSystem.h"
#include <QDebug>

NetDiskSystem::NetDiskSystem(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

	MySql::getInstance().init();//连接数据库
	
}
