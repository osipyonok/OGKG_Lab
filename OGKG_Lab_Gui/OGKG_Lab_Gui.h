#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_OGKG_Lab_Gui.h"
#include <qpainter.h>

class OGKG_Lab_Gui : public QMainWindow
{
	Q_OBJECT

public:
	OGKG_Lab_Gui(QWidget *parent = Q_NULLPTR);

protected:
	void paintEvent(QPaintEvent *event);

private:
	Ui::OGKG_Lab_GuiClass ui;
};
