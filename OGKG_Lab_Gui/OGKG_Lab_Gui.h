#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_OGKG_Lab_Gui.h"

class OGKG_Lab_Gui : public QMainWindow
{
	Q_OBJECT

public:
	OGKG_Lab_Gui(QWidget *parent = Q_NULLPTR);
	~OGKG_Lab_Gui() {}

public slots:
	void run_algo_clicked(bool checked = false);
	void pointAdded(QPoint point);

private:
	Ui::OGKG_Lab_GuiClass ui;
};
