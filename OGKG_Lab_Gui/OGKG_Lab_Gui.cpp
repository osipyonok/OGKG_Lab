#include "OGKG_Lab_Gui.h"
#include "MinimumAreaPolygonization.h"
#include <QDebug>

OGKG_Lab_Gui::OGKG_Lab_Gui(QWidget *parent)
	: QMainWindow(parent)
{
	ui->setupUi(this);
//	ui->PlotWidget->drawLine(QLine(0, 0, 130, 110));
//	ui->PlotWidget->drawLine(QLine(130, 110, 120, 15));
//	ui->PlotWidget->drawPoint(QPoint(130, 110));
	/*
	vector<int> xx = {90, 10, 140, 190, 280, 150, 280, 200, 250, 100};
	vector<int> yy = {14, 112, 80, 40, 338, 92, 160, 314, 126, 270};

	vector<pdd> v(xx.size());
	for(int i =0;i<xx.size();++i) {
		v[i].first = xx[i]; v[i].second = yy[i];
		qDebug() << xx[i] << " " << yy[i] << endl;
		ui->PlotWidget->drawPoint(QPoint(xx[i], yy[i]));
	}
	MinimumAreaPolygonization map(v);
	auto res = map.solve();

	for (int i = 0; i < res.size(); ++i) {
		qDebug() << "Segment " << res[i].first << " " << res[i].second << " " << res[(i + 1) % res.size()].first <<
			" " << res[(i + 1) % res.size()].second << endl;
		ui->PlotWidget->drawLine(QLine(res[i].first, res[i].second, res[(i + 1) % res.size()].first, res[(i + 1) % res.size()].second));
	}*/

	connect(ui->runAlgorithm, SIGNAL(clicked(bool)), this, SLOT(run_algo_clicked(bool)));
	connect(ui->PlotWidget, SIGNAL(pointAdded(QPoint)), this, SLOT(pointAdded(QPoint)));
}

void OGKG_Lab_Gui::pointAdded(QPoint point) {
	qDebug() << "Added " << point << endl;
}

void OGKG_Lab_Gui::run_algo_clicked(bool checked) {
	auto points = ui->PlotWidget->getPoints();
	ui->PlotWidget->clearLines();
	vector<pdd> v(points.size());
	for (int i = 0; i < v.size(); ++i) {
		v[i].first = points[i].x();
		v[i].second = points[i].y();
		qDebug() << v[i].first << " " << v[i].second << endl;
	}

	MinimumAreaPolygonization map(v);
	auto res = map.solve();

	for (int i = 0; i < res.size(); ++i) {
		qDebug() << "Segment " << res[i].first << " " << res[i].second << " " << res[(i + 1) % res.size()].first <<
			" " << res[(i + 1) % res.size()].second << endl;
		ui->PlotWidget->drawLine(QLine(res[i].first, res[i].second, res[(i + 1) % res.size()].first, res[(i + 1) % res.size()].second));
	}

	ui->PlotWidget->repaint();
}