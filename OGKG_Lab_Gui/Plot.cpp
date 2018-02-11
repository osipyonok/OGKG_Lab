#include "Plot.h"
#include <qpainter.h>
#include <QPaintEvent>

Plot::Plot(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	QPalette Pal(palette());
	Pal.setColor(QPalette::Background, Qt::white);
	this->setAutoFillBackground(true);
	this->setPalette(Pal);
}

Plot::~Plot()
{
}

void Plot::drawPoint(const QPoint & point) {
	this->pointsToDraw.push_back(QPoint(point.x(), this->size().height() - point.y()));
}

void Plot::drawLine(const QLine & line) {
	this->linesToDraw.push_back(QLine(line.x1(), this->size().height() - line.y1(),
		line.x2(), this->size().height() - line.y2()));
}

void Plot::paintEvent(QPaintEvent *event) {
	Q_UNUSED(event);

	QPainter painter(this);

	painter.setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::SquareCap));
	for (const auto & l : this->linesToDraw) painter.drawLine(l);

	if (!this->is_points_hidden) {
		painter.setBrush(QBrush(Qt::red));
		for (const auto & p : this->pointsToDraw) painter.drawEllipse(p, 3, 3);
	}

	painter.end();
}

void Plot::hidePoints() {
	this->is_points_hidden = true;
	this->repaint();
}

void Plot::displayPoints() {
	this->is_points_hidden = false;
	this->repaint();
}