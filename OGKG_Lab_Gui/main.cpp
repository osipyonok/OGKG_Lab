#include "OGKG_Lab_Gui.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	OGKG_Lab_Gui w;
	w.show();
	return a.exec();
}
