#include "NetDiskClient.h"
#include <QtWidgets/QApplication>
//#include "ClientWidget.h"
//#include "UserWidget.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    /*NetDiskClient w;
    w.show();*/
	NetDiskClient::getInstance().show();

	/*ClientWidget w;
	w.show();*/

	/*UserWidget w;
	w.show();*/
    return a.exec();
}
