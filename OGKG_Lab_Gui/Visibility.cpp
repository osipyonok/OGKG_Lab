#include "Visibility.h"



Visibility::Visibility(vector<pdd> & v)
{
	this->v = v;
}


Visibility::~Visibility()
{
	this->v.clear();
	this->s.clear();
}


void Visibility::init() {
	for (int i = 1; i < v.size(); ++i) {
		if (i == 1) {
			alpha.push_back(angle_by_triangle({ v[0].first + M , v[0].second }, v[0], v[i]));
		} else {
			alpha.push_back(alpha.back() + angle_by_atan(v[i - 1], v[0], v[i]));
		}
	}
}


double Visibility::angle_by_atan(const pdd & a, const pdd & b, const pdd & c) {
	pdd oa = { b.first - a.first, b.second - a.second };
	pdd ob = { b.first - c.first, b.second - c.second };
	return atan2(oa.first * ob.second - oa.second * ob.first, oa.first * ob.first + oa.second * ob.second);
}


double Visibility::angle_by_triangle(const pdd & a, const pdd & b, const pdd & c) {
	double k1 = (b.second - a.second) / (b.first - a.first);
	double k2 = (c.second - b.second) / (c.first - b.first);
	double k3 = (a.second - c.second) / (a.first - c.first);

	double al = abs(atan((k1 - k3) / (1 + k1 * k3)));
	double be = abs(atan((k2 - k1) / (1 + k2 * k1)));
	double ga = abs(atan((k2 - k3) / (1 + k2 * k3)));

	pdd oa = { b.first - a.first, b.second - a.second };
	pdd ob = { b.first - c.first, b.second - c.second };
	double val = atan2(oa.first * ob.second - oa.second * ob.first, oa.first * ob.first + oa.second * ob.second);
	if (val < 0) val = 2 * PI + val;
	if (val == 0) val = 2 * PI;
	return val;
}


int Visibility::orientation(pdd a, pdd b, pdd c) {
	double res = (b.second - a.second)*(c.first - b.first) -
		(c.second - b.second)*(b.first - a.first);

	if (abs(res) < eps) return 0;
	if (res > 0) return 1;
	return -1;
}


pdd Visibility::intersect(const pdd & A, const pdd & B, const pdd & C, const pdd & D) {
	double a1 = A.second - B.second;
	double b1 = B.first - A.first;
	double c1 = A.first * B.second - B.first * A.second;

	double a2 = C.second - D.second;
	double b2 = D.first - C.first;
	double c2 = C.first * D.second - D.first * C.second;

	double x = -1488, y = -1488;

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


vector<pdd> Visibility::VisibilityFromFirstPoint() { // 2 - 5 ok
	if (done == true) return this->s;

	init();

	s.push_back(v[0]);
	s.push_back(v[1]);

	vector<double> a{ 0 , alpha[1] };
	int k = 3;

	while (k < v.size() + 1)
	{
		double cur = alpha[k - 1];
		double last = a.back();

		if (cur >= last) {
			if (s.size() && s.back() != v[k - 1]) {
				s.push_back(v[k - 1]);
				a.push_back(cur);
			}
			++k;
			continue;
		} else {
			int ori = orientation(s[s.size() - 2], s.back(), v[k - 1]);
			if (ori == 1) { // right turn
				while (alpha[k - 1] < last) ++k;

				pdd A = v[k - 2];
				pdd B = v[k - 1];
				pdd C = v[0];
				pdd D = s.back();

				pdd E = intersect(A, B, C, D);

				if (s.size() && s.back() != E) {
					a.push_back(a.back() + angle_by_atan(s.back(), v[0], E));
					s.push_back(E);
				//	a.push_back(angle_by_triangle({ v[0].first + M, v[0].second }, v[0], E));
				}

				if (s.size() && s.back() != v[k - 1]) {
					s.push_back(v[k - 1]);
					a.push_back(alpha[k - 1]);
				}

				++k;
			} else if (ori == -1) { // left turn
				while (a[a.size() - 2] > cur) {
					a.pop_back();
					s.pop_back();
				}

				pdd A = s.back();
				pdd B = s[s.size() - 2];
				pdd C = v[0];
				pdd D = v[k - 1];

				pdd E = intersect(A, B, C, D);

				a.pop_back();
				s.pop_back();

				if (s.size() && s.back() != E) {
					a.push_back(a.back() + angle_by_atan(s.back(), v[0], E));
					s.push_back(E);
				//	a.push_back(angle_by_triangle({ v[0].first + M, v[0].second }, v[0], E));
				}

				if (s.size() && s.back() != v[k - 1]) {
					s.push_back(v[k - 1]);
					a.push_back(alpha[k - 1]);
				}

				++k;
			} else if (ori == 0) { // no turn 
				if (s.size() && s.back() != v[k - 1]) {
					s.push_back(v[k - 1]);
					a.push_back(cur);
				}

				++k;
			}
		}
	}

	done = true;
	return this->s;
}
