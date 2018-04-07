#include "OGKG_Lab_Gui.h"
#include "MinimumAreaPolygonization.h"
#include <QDebug>
#include <qmessagebox.h>
#include <QVector>
#include <BasicGeometry.h>

using namespace BasicGeometry;


OGKG_Lab_Gui::OGKG_Lab_Gui(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

//	RANDOMIZE;
	srand(time(0));

	ui.newPointsAmount->setMinimum(0);
	ui.newPointsAmount->setMaximum(10000);

	/*
	ui.PlotWidget->drawPoint(QPoint(60, 283));
	ui.PlotWidget->drawPoint(QPoint(129, 352));
	ui.PlotWidget->drawPoint(QPoint(52, 464));
	ui.PlotWidget->drawPoint(QPoint(202, 474));
	ui.PlotWidget->drawPoint(QPoint(154, 417));
	
	
	ui.PlotWidget->drawPoint(QPoint(259, 450));
	ui.PlotWidget->drawPoint(QPoint(294, 483));
	ui.PlotWidget->drawPoint(QPoint(361, 483));
	ui.PlotWidget->drawPoint(QPoint(275, 349));
	ui.PlotWidget->drawPoint(QPoint(300, 447));
	*/

	/*
	ui.PlotWidget->drawPoint(QPoint(100, 100));
	ui.PlotWidget->drawPoint(QPoint(110, 110));
	ui.PlotWidget->drawPoint(QPoint(120, 120));
	ui.PlotWidget->drawPoint(QPoint(130, 130));
	ui.PlotWidget->drawPoint(QPoint(220, 250));
	*/


	connect(ui.runAlgorithm, SIGNAL(clicked(bool)), this, SLOT(run_algo_clicked(bool)));
	connect(ui.PlotWidget, SIGNAL(pointAdded(QPoint)), this, SLOT(pointAdded(QPoint)));
	connect(ui.generateButton, SIGNAL(clicked(bool)), this, SLOT(generateNewPoints()));
	connect(ui.clearPoints, SIGNAL(clicked(bool)), this, SLOT(clearPoints()));
}


auto comp = [](const QPoint & a, const QPoint & b) { return a.x() < b.x() || (a.x() == b.x() && a.y() < b.y()); };
set<QPoint, decltype(comp)> used_points(comp);

set<QPoint, decltype(comp)>::iterator Prev(set<QPoint, decltype(comp)>::iterator & it) {
	if (used_points.begin() == it) return used_points.end();
	return prev(it);
}


set<QPoint, decltype(comp)>::iterator Next(set<QPoint, decltype(comp)>::iterator & it) {
	if (used_points.end() == it) return it;
	return next(it);
}


void OGKG_Lab_Gui::generateNewPoints() {
	int n = ui.newPointsAmount->value();
	int x_max = n + ui.PlotWidget->getPoints().size() > MAX_PONITS_TO_PLOT ? 50000 : (int)ui.PlotWidget->size().width();
	int y_max = n + ui.PlotWidget->getPoints().size() > MAX_PONITS_TO_PLOT ? 50000 : (int)ui.PlotWidget->size().height();
	

	while (n--) {
		int x = rand() % x_max;
		int y = rand() % y_max;
		QPoint point(x, y);

		auto it = used_points.lower_bound(point);
		if (it != used_points.end() && *it == point) {
			++n;
			continue;
		}

		// prev2 prev1 point next1 next2

		auto prev1 = Prev(it);
		auto prev2 = Prev(prev1);

		auto next1 = it;
		auto next2 = Next(next1);

		bool pr2pr1po, pr1pone1, pone1ne2;

		if (prev1 == used_points.end() || prev2 == used_points.end()) {
			pr2pr1po = true;
		} else {
			pr2pr1po = !at_one_line(*prev2, *prev1, point);
		}

		if (prev1 == used_points.end() || next1 == used_points.end()) {
			pr1pone1 = true;
		} else {
			pr1pone1 = !at_one_line(*prev1, point, *next1);
		}

		if (next1 == used_points.end() || next2 == used_points.end()) {
			pone1ne2 = true;
		} else {
			pone1ne2 = !at_one_line(point, *next1, *next2);
		}

		if (pr2pr1po && pr1pone1 && pone1ne2) {
			used_points.insert(point);
			ui.PlotWidget->drawPoint(point);
		}
		else ++n;
	}
	ui.PlotWidget->repaint();
}


void OGKG_Lab_Gui::pointAdded(QPoint point) {
	qDebug() << "Added " << point << endl;
}


void OGKG_Lab_Gui::clearPoints() {
	srand(0);

	used_points.clear();
	this->ui.PlotWidget->clearAll();
	this->ui.PlotWidget->repaint();
}


void OGKG_Lab_Gui::save_as_png(const vector<pair<double, double>>& poly)
{
	double max_x = numeric_limits<double>::min();
	double max_y = numeric_limits<double>::min();

	for (const auto & e : poly) {
		max_x = max(max_x, e.first);
		max_y = max(max_y, e.second);
	}

	QImage image(QSize(max_x + 30, max_y + 30), QImage::Format_RGB32);
	QPainter img_painter(&image);
	img_painter.setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::SquareCap));


	QPainterPath path;
	path.moveTo(QPoint(poly[0].first, poly[0].second));
	for (int i = 0; i < poly.size(); ++i){
		path.lineTo(QPoint(poly[(i + 1) % poly.size()].first, poly[(i + 1) % poly.size()].second));
	}
	//img_painter.setPen(Qt::NoPen);
	img_painter.fillPath(path, QBrush(QColor("green")));

	//QPoint *pts = new QPoint[poly.size()];
	//for (int i = 0; i < poly.size(); ++i) {
	//	pts[i] = QPoint(poly[i].first, poly[i].second);
	//}
	//img_painter.drawPolygon(pts, poly.size(), Qt::WindingFill);


	img_painter.setBrush(QBrush(Qt::red));
	for (const auto & e : poly) {
	//	img_painter.drawEllipse(QPoint(e.first, e.second), 2, 2);
	}
	img_painter.end();
	image.save("Polygon.png");
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
		ui.PlotWidget->clearAll();

		for (int i = 0; i < res.size(); ++i) {
			ui.PlotWidget->drawPoint(QPoint(res[i].first, res[i].second));
			ui.PlotWidget->drawLine(QLine(res[i].first , res[i].second, res[(i + 1) % res.size()].first, res[(i + 1) % res.size()].second));
		}
		ui.PlotWidget->repaint();
		save_as_png(res);
	}
	catch (string ex) {
		QMessageBox mb(QString::fromLocal8Bit("Помикла"), 
			QString::fromLocal8Bit("На вхiд алгоритму мають задаватися хоча б 3 точки, \
			що не належать однiй прямiй!"), QMessageBox::Warning, QMessageBox::Ok, 
			QMessageBox::Escape, QMessageBox::NoButton);
		mb.exec();
	}
}
