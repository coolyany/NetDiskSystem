#include "NetDiskServer.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    NetDiskServer w;
    w.show();
    return a.exec();
}
