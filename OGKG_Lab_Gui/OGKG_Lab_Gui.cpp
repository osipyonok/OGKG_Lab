#include "OGKG_Lab_Gui.h"

OGKG_Lab_Gui::OGKG_Lab_Gui(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	this->repaint();
}

void OGKG_Lab_Gui::paintEvent(QPaintEvent *event) {
	Q_UNUSED(event);

	QPainter painter(this);
	painter.setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::FlatCap));
	painter.setBrush(QBrush(Qt::red, Qt::SolidPattern));
	
	QImage img(QSize(150, 150), QImage::Format_ARGB32);
	painter.drawLine(0, 0, 130, 110);

}
