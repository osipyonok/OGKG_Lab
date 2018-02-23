#pragma once

#include <vector>
#include <cmath>
#include <algorithm>

#define PI 3.1415926535897932384626433832795028841971
#define INF 10110101.0
#define eps 1e-6

namespace BasicGeometry {


	using namespace std;
#define pdd pair<double, double>
	int orientation(const pdd & a, const pdd & b, const pdd & c);

	int orientation_(const pdd & a, const pdd & b, const pdd & c);


	inline double det2(double a, double b, double c, double d) {
		return a * d - b * c;
	}

	bool doubles_equal(double a, double b, double EPS = eps);

	bool points_equal(pdd a, pdd b, double EPS = eps);


	pdd lines_intersect(const pdd & A, const pdd & B, const pdd & C, const pdd & D);

	
	namespace SegmentIntersection {
		inline bool  intersect_1(double & a, double & b, double & c, double & d) {
			if (a > b) swap(a, b);
			if (c > d) swap(c, d);
			return max(a, c) <= min(b, d);
		}


		inline bool  between(double a, double b, double c) {
			return min(a, b) <= c + eps && c <= max(a, b) + eps;
		}


		bool intersect(pdd & a, pdd & b, pdd & c, pdd & d);
	}


	inline bool intersect(pdd a, pdd b, pdd c, pdd d) {
		return SegmentIntersection::intersect(a, b, c, d);
	}
}