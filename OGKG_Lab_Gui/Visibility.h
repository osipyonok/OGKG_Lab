#pragma once

#include <vector>
#include <cmath>
#include <qDebug>

using namespace std;

#define pdd pair<double , double>
#define PI 3.14159265358979323846
#define M 10000

const double eps = 1e-10;

class Visibility
{
public:
	Visibility(vector<pdd> & v);
	~Visibility();
	vector<pdd> VisibilityFromFirstPoint();

private:
	vector<pdd> v, s;
	bool done = false;
	vector<double> alpha{ 0 };

	void init();
	double angle_by_atan(const pdd & a, const pdd & b, const pdd & c);
	double angle_by_triangle(const pdd & a, const pdd & b, const pdd & c);
	pdd intersect(const pdd & A, const pdd & B, const pdd & C, const pdd & D);
	int orientation(pdd a, pdd b, pdd c);

};

