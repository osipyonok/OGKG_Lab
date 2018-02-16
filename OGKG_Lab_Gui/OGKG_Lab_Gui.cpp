#include "OGKG_Lab_Gui.h"
#include "MinimumAreaPolygonization.h"
#include <QDebug>
#include <qmessagebox.h>
#include <QVector>

OGKG_Lab_Gui::OGKG_Lab_Gui(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

//	RANDOMIZE;

	ui.newPointsAmount->setMinimum(0);
	ui.newPointsAmount->setMaximum(15000);

	connect(ui.runAlgorithm, SIGNAL(clicked(bool)), this, SLOT(run_algo_clicked(bool)));
	connect(ui.PlotWidget, SIGNAL(pointAdded(QPoint)), this, SLOT(pointAdded(QPoint)));
	connect(ui.generateButton, SIGNAL(clicked(bool)), this, SLOT(generateNewPoints()));
}

void OGKG_Lab_Gui::generateNewPoints() {
	int n = ui.newPointsAmount->value();
	int x_max = n + ui.PlotWidget->getPoints().size() > MAX_PONITS_TO_PLOT ? 50000 : (int)ui.PlotWidget->size().width();
	int y_max = n + ui.PlotWidget->getPoints().size() > MAX_PONITS_TO_PLOT ? 50000 : (int)ui.PlotWidget->size().height();
	while (n--) {
		int x = rand() % x_max;
		int y = rand() % y_max;
		ui.PlotWidget->drawPoint(QPoint(x, y));
	}
	ui.PlotWidget->repaint();
}

void OGKG_Lab_Gui::pointAdded(QPoint point) {
	qDebug() << "Added " << point << endl;
}

void OGKG_Lab_Gui::run_algo_clicked(bool checked) {
	auto points = ui.PlotWidget->getPoints();
	ui.PlotWidget->clearLines();
	vector<pdd> v(points.size());
	for (int i = 0; i < v.size(); ++i) {
		v[i].first = points[i].x();
		v[i].second = points[i].y();
		qDebug() << v[i].first << " " << v[i].second << endl;
	}

	MinimumAreaPolygonization map(v);
	try {
		auto res = map.solve();
		for (int i = 0; i < res.size(); ++i) {
			qDebug() << "Segment " << res[i].first << " " << res[i].second << " " << res[(i + 1) % res.size()].first <<
				" " << res[(i + 1) % res.size()].second << endl;
			ui.PlotWidget->drawLine(QLine(res[i].first, res[i].second, res[(i + 1) % res.size()].first, res[(i + 1) % res.size()].second));

			ui.PlotWidget->repaint();
		}
	}
	catch (string ex) {
		QMessageBox mb(QString::fromLocal8Bit("Помикла"), 
			QString::fromLocal8Bit("На вхiд алгоритму мають задаватися хоча б 3 точки, \
			що не належать однiй прямiй!"), QMessageBox::Warning, QMessageBox::Ok, 
			QMessageBox::Escape, QMessageBox::NoButton);
		mb.exec();
	}
}
