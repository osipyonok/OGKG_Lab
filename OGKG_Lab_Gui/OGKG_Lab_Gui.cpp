#include "OGKG_Lab_Gui.h"
#include "MinimumAreaPolygonization.h"

OGKG_Lab_Gui::OGKG_Lab_Gui(QWidget *parent)
	: QMainWindow(parent)
{
	ui->setupUi(this);
	ui->PlotWidget->drawLine(QLine(0, 0, 130, 110));
	ui->PlotWidget->drawLine(QLine(130, 110, 120, 15));
	ui->PlotWidget->drawPoint(QPoint(130, 110));
}