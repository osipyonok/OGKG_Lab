#pragma once

#include <QWidget>
#include "ui_Plot.h"
#include <qpainter.h>
#include <QPoint>
#include <QLine>
#include <QVector>

class Plot : public QWidget
{
	Q_OBJECT

public:
	Plot(QWidget *parent = Q_NULLPTR);
	void drawPoint(const QPoint & point);
	void drawLine(const QLine & line);
	void hidePoints();
	void displayPoints();
	~Plot();

protected:
	void paintEvent(QPaintEvent *event);

private:
	Ui::Plot ui;
	QVector<QPoint> pointsToDraw;
	QVector<QLine> linesToDraw;
	bool is_points_hidden = false;
}; 
