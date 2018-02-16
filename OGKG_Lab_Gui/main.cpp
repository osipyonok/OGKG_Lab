#include "OGKG_Lab_Gui.h"
#include <QtWidgets/QApplication>
#include <qtextcodec.h>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());
	OGKG_Lab_Gui w;
	w.show();
	return a.exec();
}
