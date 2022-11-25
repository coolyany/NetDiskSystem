#include "NetDiskServer.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    NetDiskServer w;

	/*MySql::getInstance().init();
	system("pause");
	return 0;*/

    //w.show();
    return a.exec();
}
