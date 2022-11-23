#include "NetDiskClient.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    NetDiskClient w;
    w.show();
    return a.exec();
}
