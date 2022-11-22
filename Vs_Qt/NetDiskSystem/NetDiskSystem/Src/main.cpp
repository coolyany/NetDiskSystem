#include "NetDiskSystem.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    NetDiskSystem w;
    w.show();
    return a.exec();
}
