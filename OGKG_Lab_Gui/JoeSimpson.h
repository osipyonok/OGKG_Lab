#pragma once

#ifndef JOE_SIMPSON
#define JOE_SIMPSON

#include <vector>
#include <cmath>
#include <qDebug>
#include "BasicGeometry.h"

using namespace std;
using namespace BasicGeometry;



class JoeSimpson
{
public:
	JoeSimpson(vector<pdd> & poly);
	vector<pdd> run();

	~JoeSimpson();

private:
	struct JoeSimpsonPoint
	{
		pdd point;
		pdd unrotated;
		bool is_original;
		JoeSimpsonPoint(pdd & point, bool is_original, pdd unrotated = { 0, 0 }) {
			this->point = point;
			this->is_original = is_original;
			this->unrotated = unrotated;
		}
		bool operator!=(const JoeSimpsonPoint & other) const {
			return this->point != other.point;
		}
	};

	bool is_solved;
	vector<pdd> original_points;
	vector<pdd> poly;
	vector<pdd> stack;
	vector<double> alpha{ 0 };
	pdd delta;
	double rotation_angle;

	void prepare();
	void init_alpha();
	void rotate_point(pdd & p, double angle);
	void rotate_polygon(vector<pdd> & poly, double angle);
	bool isCCW(const vector<pdd> & poly);
	pdd from_polar(double r, double theta);
	pdd to_polar(pdd p);
	double angle(const pdd & a, const pdd & b, const pdd & c);
	bool intersect_with_w(pdd & A, pdd & B, pdd & C, pdd & w);

	void visibility_polygon(vector<JoeSimpsonPoint> & v, vector<JoeSimpsonPoint> & s, int & t);//from poly[0]
	void advance(vector<JoeSimpsonPoint> & v, vector<JoeSimpsonPoint> & s, vector<double> & a,
		int & t, int & i, string & vpcase, bool & ccw, pdd & w);
	void retard(vector<JoeSimpsonPoint> & v, vector<JoeSimpsonPoint> & s, vector<double> & a,
		int & t, int & i, string & vpcase, bool & ccw, pdd & w);
	void scan(vector<JoeSimpsonPoint> & v, vector<JoeSimpsonPoint> & s, vector<double> & a,
		int & t, int & i, string & vpcase, bool & ccw, pdd & w);



};

#endif JOE_SIMPSON