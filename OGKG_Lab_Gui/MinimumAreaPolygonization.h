#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <set>
#include <primitives.hpp>
#include <floats.hpp>
#include <vector2.hpp>
#include <visibility.hpp>


#define pdd pair<double,double>

using namespace std;

#pragma once
class MinimumAreaPolygonization
{
public:
	MinimumAreaPolygonization(vector<pdd> points);
	vector<pdd> solve();
	~MinimumAreaPolygonization();

private:
	vector<pdd> points;
	void preprocess(vector<pdd> & v);
	inline double cos_angle(const pdd & a, const pdd & b);
	inline double det(double a, double b, double c, double d);
	inline bool intersect_1(double a, double b, double c, double d);
	inline bool between(double a, double b, double c);
	inline int quad(pdd p);
	inline int orientation(pdd a, pdd b, pdd c);
	inline bool check_segments(pair<pdd, pdd> seg1, pair<pdd, pdd> seg2);
	bool intersect(pdd a, pdd b, pdd c, pdd d);
	bool is_polygon(vector<pair<pdd, pdd>> & pol);
	void brute_optimal_polygon(vector<pdd> & v, int l, int r);
	vector<pdd> sort_points(vector<pdd> ret);
	double polygon_area(vector<pdd> & v);
	vector<pdd> brute_hull(vector<pdd> a);
	vector<pdd> merger(vector<pdd> a, vector<pdd> b,
		pair<pdd, pdd> upperTangent, pair<pdd, pdd> lowerTangent);
	int leftmost_ponint_index(const vector<pdd> & poly);
	int rightmost_ponint_index(const vector<pdd> & poly);
	vector<pdd> divide_hull(vector<pdd> a, bool first = true);
	pair<pdd, pdd> findUpperTangent(vector<pdd> a, vector<pdd> b, int rma, int lmb);
	pair<pdd, pdd> findLowerTangent(vector<pdd> a, vector<pdd> b, int rma, int lmb);
	vector<pdd> merge_polygons(vector<pdd> & v, int l, int r, int mid,
		vector<pdd> & lhull, vector<pdd> & rhull);
	vector<pdd> solve(vector<pdd> & v, int l, int r);
	void minimum_quadrilateral_visibility(vector<pair<pdd, pdd>> & seg_l, vector<pair<pdd, pdd>> & seg_r,
		pair<pdd, pdd> & upperTangent, pair<pdd, pdd> & lowerTangent, pair<pdd, pdd> & u1, pair<pdd, pdd> & u2);
	void minimum_quadrilateral_brute(vector<pair<pdd, pdd>> & seg_l, vector<pair<pdd, pdd>> & seg_r
			, pair<pdd, pdd> & u1, pair<pdd, pdd> & u2);

	const double eps = 1e-10;



};

