#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_OGKG_Lab_Gui.h"


#define MAX_PONITS_TO_PLOT 100
#define RANDOMIZE srand(std::chrono::duration_cast<std::chrono::nanoseconds>((std::chrono::high_resolution_clock::now().time_since_epoch())).count() + time(0))

class OGKG_Lab_Gui : public QMainWindow
{
	Q_OBJECT

public:
	OGKG_Lab_Gui(QWidget *parent = Q_NULLPTR);
	~OGKG_Lab_Gui() {}

public slots:
	void run_algo_clicked(bool checked = false);
	void pointAdded(QPoint point);
	void generateNewPoints();

private:
	Ui::OGKG_Lab_GuiClass ui;
};
