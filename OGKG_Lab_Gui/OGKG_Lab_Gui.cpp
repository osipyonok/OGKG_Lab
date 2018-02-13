#include "OGKG_Lab_Gui.h"
#include "MinimumAreaPolygonization.h"
#include <QDebug>

OGKG_Lab_Gui::OGKG_Lab_Gui(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	connect(ui.runAlgorithm, SIGNAL(clicked(bool)), this, SLOT(run_algo_clicked(bool)));
	connect(ui.PlotWidget, SIGNAL(pointAdded(QPoint)), this, SLOT(pointAdded(QPoint)));
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
	auto res = map.solve();

	for (int i = 0; i < res.size(); ++i) {
		qDebug() << "Segment " << res[i].first << " " << res[i].second << " " << res[(i + 1) % res.size()].first <<
			" " << res[(i + 1) % res.size()].second << endl;
		ui.PlotWidget->drawLine(QLine(res[i].first, res[i].second, res[(i + 1) % res.size()].first, res[(i + 1) % res.size()].second));
	}

	ui.PlotWidget->repaint();
}
