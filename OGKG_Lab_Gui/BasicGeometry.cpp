#include "BasicGeometry.h"
#include <qdebug.h>

//using namespace BasicGeometry;

namespace BasicGeometry {

	bool BasicGeometry::doubles_equal(double a, double b, double EPS) {
		return abs(a - b) < EPS;
	}

	bool BasicGeometry::points_equal(pdd a, pdd b, double EPS) {
		return BasicGeometry::doubles_equal(a.first, b.first) && BasicGeometry::doubles_equal(a.second, b.second);
	}


	int BasicGeometry::orientation(const pdd & a, const pdd & b, const pdd & c) {
		double res = (b.second - a.second)*(c.first - b.first) -
			(c.second - b.second)*(b.first - a.first);

		if (abs(res) < eps)
			return 0;
		if (res > 0)
			return 1;
		return -1;
	}

	int BasicGeometry::orientation_(const pdd & a, const pdd & b, const pdd & c) {
		return orientation(a, b, c);
	}

	pdd BasicGeometry::lines_intersect(const pdd & A, const pdd & B, const pdd & C, const pdd & D) {//lines are not parallel
		double a1 = A.second - B.second;
		double b1 = B.first - A.first;
		double c1 = A.first * B.second - B.first * A.second;

		double a2 = C.second - D.second;
		double b2 = D.first - C.first;
		double c2 = C.first * D.second - D.first * C.second;

		double x = 1488, y = 1488;

		if (a1 != 0) {
			if (-a2 * b1 + a1 * b2 != 0) {
				x = (b2 * c1 - b1 * c2) / (a2 * b1 - a1 * b2);
				y = (-a2 * c1 + a1 * c2) / (a2 * b1 - a1 * b2);
			}
			else if (a1 * b2 - a2 * b1 == 0) {
				//no solution
				qDebug() << "WOW\n";
			}
		}
		else {
			if (a2 != 0) {
				if (b1 != 0) {
					x = (b2 * c1 - b1 * c2) / (a2 * b1);
					y = -c1 / b1;
				}
				else {
					if (c1 == 0) {
						y = 1;
						x = (-c2 / a2) - (b2 / a2) * y;
					}
					else {
						// no solution
						qDebug() << "WOW\n";
					}
				}
			}
			else {
				// TODO
				qDebug() << "TODO\n";
			}
		}
		return { x , y };
	}


	bool BasicGeometry::SegmentIntersection::intersect(pdd & a, pdd & b, pdd & c, pdd & d) {
		double A1 = a.second - b.second, B1 = b.first - a.first, C1 = -A1*a.first - B1*a.second;
		double A2 = c.second - d.second, B2 = d.first - c.first, C2 = -A2*c.first - B2*c.second;
		double zn = det2(A1, B1, A2, B2);
		if (zn != 0) {
			double x = -det2(C1, B1, C2, B2) * 1.0 / zn;
			double y = -det2(A1, C1, A2, C2) * 1.0 / zn;
			return between(a.first, b.first, x) && between(a.second, b.second, y)
				&& between(c.first, d.first, x) && between(c.second, d.second, y);
		}
		else
			return det2(A1, C1, A2, C2) == 0 && det2(B1, C1, B2, C2) == 0
			&& intersect_1(a.first, b.first, c.first, d.first)
			&& intersect_1(a.second, b.second, c.second, d.second);
	}
}