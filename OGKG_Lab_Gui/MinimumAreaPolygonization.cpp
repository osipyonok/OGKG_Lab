#include "MinimumAreaPolygonization.h"
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <boost/functional/hash.hpp>
#include <qdebug.h>

using namespace std;
using namespace geometry;
using vector_type = geometry::vec2;
using segment_type = geometry::line_segment<vector_type>;
using segment_comparer_type = geometry::line_segment_dist_comparer<vector_type>;
using angle_comparer_type = geometry::angle_comparer<vector_type>;

pdd middle = { 0, 0 };
inline int quad(pdd p);

MinimumAreaPolygonization::MinimumAreaPolygonization(vector<pdd> points)
{
	this->points = points;
}

vector<pdd> MinimumAreaPolygonization::solve() {
	preprocess(points);
	if (points.size() <= 2) throw string("I need more points!");
	solve(points, 0, points.size() - 1);
	return points;
}

MinimumAreaPolygonization::~MinimumAreaPolygonization()
{
}

double MinimumAreaPolygonization::polygon_area(vector<pdd> & v) {
	double a = 0.0;
	int sz = v.size();
	for (int i = 0; i < sz; ++i) {
		a += v[i].first * (v[(i + 1) % sz].second - v[(sz + i - 1) % sz].second);
	}
	return 0.5 * abs(a);
}

inline double MinimumAreaPolygonization::cos_angle(const pdd & a, const pdd & b) {
	if (a.first == 0.0 && b.first == 0.0 && a.second == 0.0 && b.second == 0.0) return 1;
	double ab = a.first * b.first + a.second * b.second;
	double aa = a.first * a.first + a.second * a.second;
	double bb = b.first * b.first + b.second * b.second;
	return abs(ab) / sqrt(aa * bb);
}

inline bool MinimumAreaPolygonization::intersect_1(double a, double b, double c, double d) {
	if (a > b) swap(a, b);
	if (c > d) swap(c, d);
	return max(a, c) <= min(b, d);
}

inline bool MinimumAreaPolygonization::between(double a, double b, double c) {
	return min(a, b) <= c + eps && c <= max(a, b) + eps;
}

bool MinimumAreaPolygonization::intersect(pdd a, pdd b, pdd c, pdd d) {
	int A1 = a.second - b.second, B1 = b.first - a.first, C1 = -A1*a.first - B1*a.second;
	int A2 = c.second - d.second, B2 = d.first - c.first, C2 = -A2*c.first - B2*c.second;
	int zn = det(A1, B1, A2, B2);
	if (zn != 0) {
		double x = -det(C1, B1, C2, B2) * 1. / zn;
		double y = -det(A1, C1, A2, C2) * 1. / zn;
		return between(a.first, b.first, x) && between(a.second, b.second, y)
			&& between(c.first, d.first, x) && between(c.second, d.second, y);
	}
	else
		return det(A1, C1, A2, C2) == 0 && det(B1, C1, B2, C2) == 0
		&& intersect_1(a.first, b.first, c.first, d.first)
		&& intersect_1(a.second, b.second, c.second, d.second);
}

bool MinimumAreaPolygonization::is_polygon(vector<pair<pdd, pdd>> & pol) {
	bool res = true;
	int n = pol.size();
	for (int i = 0; i < n; ++i) {
		int nxt = (i + 1) % n, prv = (i + n - 1) % n;
		res &= pol[i].second == pol[nxt].first;
		res &= pol[i].first == pol[prv].second;
		nxt = (nxt + 1) % n;
		while (nxt != prv) {
			res &= !(intersect(pol[i].first, pol[i].second, pol[nxt].first, pol[nxt].second));
			nxt = (nxt + 1) % n;
		}
	}
	return res;
}

inline int quad(pdd p) {
	if (p.first >= 0 && p.second >= 0)
		return 1;
	if (p.first <= 0 && p.second >= 0)
		return 2;
	if (p.first <= 0 && p.second <= 0)
		return 3;
	return 4;
}

inline int MinimumAreaPolygonization::orientation(pdd a, pdd b, pdd c) {
	int res = (b.second - a.second)*(c.first - b.first) -
		(c.second - b.second)*(b.first - a.first);

	if (res == 0)
		return 0;
	if (res > 0)
		return 1;
	return -1;
}

bool compare(pdd p1, pdd q1) {
	pdd p = make_pair(p1.first - middle.first,
		p1.second - middle.second);
	pdd q = make_pair(q1.first - middle.first,
		q1.second - middle.second);

	int one = quad(p);
	int two = quad(q);

	if (one != two)
		return (one < two);
	return (p.second * q.first < q.second * p.first);
}

vector<pdd> MinimumAreaPolygonization::brute_hull(vector<pdd> a) {
	set<pdd> s;

	for (int i = 0; i< a.size(); i++)
	{
		for (int j = i + 1; j< a.size(); j++)
		{
			double x1 = a[i].first, x2 = a[j].first;
			double y1 = a[i].second, y2 = a[j].second;
			double a1 = y1 - y2;
			double b1 = x2 - x1;
			double c1 = x1 * y2 - y1 * x2;
			int pos = 0, neg = 0;
			for (int k = 0; k<a.size(); k++){
				if (a1 * a[k].first + b1 * a[k].second + c1 <= 0) ++neg;
				if (a1 * a[k].first + b1 * a[k].second + c1 >= 0) ++pos;
			}
			if (pos == a.size() || neg == a.size()){
				s.insert(a[i]);
				s.insert(a[j]);
			}
		}
	}

	vector<pdd>ret;
	for (auto & e : s) ret.push_back(e);

	return sort_points(ret);
}

vector<pdd> MinimumAreaPolygonization::merger(vector<pdd> a, vector<pdd> b, 
	pair<pdd, pdd> upperTangent, pair<pdd, pdd> lowerTangent) {
	int n1 = a.size(), n2 = b.size();

	int uppera = 0, upperb = 0;
	while (a[uppera] != upperTangent.first) ++uppera;
	while (b[upperb] != upperTangent.second) ++upperb;

	int lowera = 0, lowerb = 0;
	while (a[lowera] != lowerTangent.first) ++lowera;
	while (b[lowerb] != lowerTangent.second) ++lowerb;

	vector<pdd> ret;

	int nxt = uppera;
	ret.push_back(a[uppera]);
	while (nxt != lowera) {
		nxt = (nxt + 1) % n1;
		ret.push_back(a[nxt]);
	}

	nxt = lowerb;
	ret.push_back(b[lowerb]);
	while (nxt != upperb) {
		nxt = (nxt+ 1) % n2;
		ret.push_back(b[nxt]);
	}

	return ret;
}

int MinimumAreaPolygonization::leftmost_ponint_index(const vector<pdd> & poly) {
	int n = poly.size();
	int ind = 0;
	for (int i = 1; i < n; ++i) {
		if (poly[i].first < poly[ind].first) {
			ind = i;
		}
	}
	return ind;
}

int MinimumAreaPolygonization::rightmost_ponint_index(const vector<pdd> & poly) {
	int n = poly.size();
	int ind = 0;
	for (int i = 1; i < n; ++i) {
		if (poly[i].first > poly[ind].first) {
			ind = i;
		}
	}
	return ind;
}

pair<pdd, pdd> MinimumAreaPolygonization::findUpperTangent(vector<pdd> a, vector<pdd> b, int rma, int lmb) {
	int n1 = a.size(), n2 = b.size();
	int inda = rma, indb = lmb;
	bool done = 0;

	while (!done) {
		done = 1;
		while (orientation(b[indb], a[inda], a[(inda + 1) % n1]) >= 0) 
			inda = (inda + 1) % n1;
		while (orientation(a[inda], b[indb], b[(n2 + indb - 1) % n2]) <= 0) {
			indb = (n2 + indb - 1) % n2;
			done = 0;
		}
	}

	return { a[inda], b[indb] };
}

pair<pdd, pdd> MinimumAreaPolygonization::findLowerTangent(vector<pdd> a, vector<pdd> b, int rma, int lmb) {
	int n1 = a.size(), n2 = b.size();
	int inda = rma, indb = lmb;
	bool done = 0;

	while (!done) {
		done = 1;
		while (orientation(a[inda], b[indb], b[(indb + 1) % n2]) >= 0)
			indb = (indb + 1) % n2;

		while (orientation(b[indb], a[inda], a[(n1 + inda - 1) % n1]) <= 0) {
			inda = (n1 + inda - 1) % n1;
			done = 0;
		}
	}

	return { a[inda], b[indb] };
}

inline bool MinimumAreaPolygonization::check_segments(pair<pdd, pdd> seg1, pair<pdd, pdd> seg2) {
	if (seg1.first == seg2.first
		|| seg1.first == seg2.second
		|| seg1.second == seg2.first
		|| seg1.second == seg2.second) return true; //nearby segments
	return !intersect(seg1.first, seg1.second, seg2.first, seg2.second);
}

inline void add(vector<segment_type> & L, const pair<pdd, pdd> & e) {
	segment_type seg;
	seg.a.x = e.first.first;
	seg.a.y = e.first.second;
	seg.b.x = e.second.first;
	seg.b.y = e.second.second;
	L.push_back(seg);
}

void MinimumAreaPolygonization::minimum_quadrilateral_visibility(vector<pair<pdd, pdd>> & seg_l, vector<pair<pdd, pdd>> & seg_r,
	pair<pdd, pdd> & upperTangent, pair<pdd, pdd> & lowerTangent, pair<pdd, pdd> & seg1, pair<pdd, pdd> & seg2) {
	vector<segment_type> L;

	for (auto e : seg_l) add(L, e);
	for (auto e : seg_r) add(L, e);
	add(L, upperTangent), add(L, lowerTangent);

	unordered_map<pdd, int, boost::hash<pdd>> ind;
	unordered_set<pdd, boost::hash<pdd>> set;
	for (int i = 0; i < seg_r.size(); ++i) set.insert(seg_r[i].first), set.insert(seg_r[i].second);
	int ii = 0;
	for (auto it = set.begin(); it != set.end(); ++it, ++ii) ind[*it] = ii;
	vector<unordered_set<pdd, boost::hash<pdd>>> graph(set.size());

	for (auto point : set) {
		auto poly = visibility_polygon(vector_type{ (float)point.first, (float)point.second }, L.begin(), L.end());
		int k = poly.size();
		for (int i = 0; i < k; ++i) {
			graph[ind[point]].insert({ poly[i].x, poly[i].y });
		}
	}

	double minimum_area = DBL_MAX;

	for (auto s1 : seg_l) {
		for (auto s2 : seg_r) {
			/*
			A*-------*B
	   s1	|		 |  s2
			|		 |
			C*-------*D
			*/
			pdd A = s1.first;
			pdd C = s1.second;
			pdd B = s2.first;
			pdd D = s2.second;

			if (intersect(A, B, C, D)) {
				if (intersect(A, D, C, B)) {
					continue;
				}
				swap(B, D);
			}

			bool can_see_a_from_b = graph[ind[B]].count(A) > 0;
			bool can_see_c_from_d = graph[ind[D]].count(C) > 0;

			if (can_see_a_from_b && can_see_c_from_d) {
				vector<pdd> tetragon{ A, C, D, B };
				double current_area = polygon_area(tetragon);
				if (current_area < minimum_area) {
					minimum_area = current_area;
					seg1 = { A, C };
					seg2 = { B, D };
				}
			}
		}
	}
}

void MinimumAreaPolygonization::minimum_quadrilateral_brute(vector<pair<pdd, pdd>> & seg_l, vector<pair<pdd, pdd>> & seg_r
			, pair<pdd, pdd> & u1, pair<pdd, pdd> & u2) {
	double ar = DBL_MAX;
	for (auto s1 : seg_l) {
		for (auto s2 : seg_r) {
			vector<pair<pair<pdd, pdd>, pair<pdd, pdd>>> tmp = {
				{ { s1.first , s2.second } ,{ s1.second , s2.first } },
				{ { s1.first , s2.first } ,{ s1.second , s2.second } }
			};

			for (auto el : tmp) {

				bool ok = true;

				pair<pdd, pdd> trs1 = el.first;
				pair<pdd, pdd> trs2 = el.second;
				if (intersect(trs1.first, trs1.second, trs2.first, trs2.second)) continue;

				for (auto s11 : seg_l) ok &= check_segments(s11, trs1), ok &= check_segments(s11, trs2);
				for (auto s22 : seg_r) ok &= check_segments(s22, trs2), ok &= check_segments(s22, trs1);

				if (ok) {
					vector<pdd> tra;
					tra.push_back(s1.first), tra.push_back(s1.second);
					tra.push_back(s2.second), tra.push_back(s2.first);
					double ca = polygon_area(tra);
					if (ca < ar) {
						ar = ca;
						u1 = s1;
						u2 = s2;
					}
				}
			}
		}
	}
}

vector<pdd> MinimumAreaPolygonization::merge_polygons(vector<pdd> & v, int l, int r, int mid, 
	vector<pdd> & lhull, vector<pdd> & rhull) {

	int rma = rightmost_ponint_index(lhull);
	int lmb = leftmost_ponint_index(rhull);

	auto upper = findUpperTangent(lhull, rhull, rma, lmb);
	auto lower = findLowerTangent(lhull, rhull, rma, lmb);

	int i1 = 0, i2 = mid + 1;
	while (v[i1] != lower.first)
		i1 = (i1 + 1) % (mid + 1);
	while (v[i2] != lower.second)
		++i2;
	vector<pair<pdd, pdd>> seg1, seg2;
	
	do {
		int nxt = (i1 + 1 >= mid + 1 ? l : i1 + 1);
		seg1.push_back({ v[i1], v[nxt] });
		i1 = nxt;
	} while (v[i1] != upper.first);

	do {
		int prv = (i2 - 1 <= mid ? r : i2 - 1);
		seg2.push_back({ v[i2], v[prv] });
		i2 = prv;
	} while (v[i2] != upper.second);

	double ar = DBL_MAX;
	pair<pdd, pdd> u11, u22;
	pair<pdd, pdd> u1, u2;

	minimum_quadrilateral_brute(seg1, seg2, u1, u2);

	//minimum_quadrilateral_visibility(seg1, seg2, upper, lower, u1, u2);

	/*cout << "4gon:\n";
	cout << u1.fi.fi << " " << u1.fi.se << " " << u1.se.fi << " " << u1.se.se << endl;
	cout << u2.fi.fi << " " << u2.fi.se << " " << u2.se.fi << " " << u2.se.se << endl;
	cout << "4gon's area: " << ar << endl;*/

	vector<pdd> nw;
	int it = l;
	pdd a = v[it];
	while (v[it] != u1.second) ++it;
	while (v[it] != u1.first) {
		nw.push_back(v[it]);
		//it = (it + 1) % (mid + 1);
		it = (it + 1 >= mid + 1 ? l : it + 1);
	}
	nw.push_back(u1.first);
	it = mid + 1;
	while (v[it] != u2.first) ++it;
	while (v[it] != u2.second) {
		nw.push_back(v[it]);
		it = (it + 1 > r ? mid + 1 : it + 1);
	}
	nw.push_back(u2.second);
/*	cout << "Merged polygon:\n";
	for (int i = 0; i < sz(nw); ++i) {
		cout << "Segment " << nw[i].fi << " " << nw[i].se << " " << nw[(i + 1) % sz(nw)].fi << " " << nw[(i + 1) % sz(nw)].se << nl;
	}
	cout << endl;
	cout << "Area " << area(nw) << endl;*/
	for (int i = l; i <= r; ++i) v[i] = nw[i - l];

	return merger(lhull, rhull, upper, lower);
}

vector<pdd> MinimumAreaPolygonization::divide_hull(vector<pdd> a, bool first) {
	if (first) sort(a.begin(), a.end());

	if (a.size() <= 5)
		return brute_hull(a);

	// left contains the left half points
	// right contains the right half points
	vector<pdd>left, right;
	for (int i = 0; i<a.size() / 2; i++)
		left.push_back(a[i]);
	for (int i = a.size() / 2; i<a.size(); i++)
		right.push_back(a[i]);

	// convex hull for the left and right sets
	vector<pdd> left_hull = divide_hull(left, false);
	vector<pdd> right_hull = divide_hull(right, false);

	// merging the convex hulls
//	return merger(left_hull, right_hull );
}

vector<pdd> MinimumAreaPolygonization::sort_points(vector<pdd> ret) {
	middle = { 0, 0 };
	int n = ret.size();
	for (int i = 0; i<n; i++)
	{
		middle.first += ret[i].first;
		middle.second += ret[i].second;
		ret[i].first *= n;
		ret[i].second *= n;
	}
	sort(ret.begin(), ret.end(), compare);
	for (int i = 0; i<n; i++)
		ret[i] = make_pair(ret[i].first / n, ret[i].second / n);

	return ret;
}

void MinimumAreaPolygonization::brute_optimal_polygon(vector<pdd> & v, int l, int r) {
	vector<pdd> p(r - l + 1);
	for (int i = l; i <= r; ++i) p[i - l] = v[i];
	sort(p.begin(), p.end());
	vector<pdd> best_p(r - l + 1);
	double best_a = DBL_MAX;
	do {
		auto cur = p;
		cur.push_back(cur.front());
		vector<pair<pdd, pdd>> sides;
		for (int i = 0; i < cur.size() - 1; ++i) {
			sides.push_back({ cur[i], cur[i + 1] });
		}
		if (is_polygon(sides)) {
			double a = polygon_area(p);
			if (a < best_a) {
				best_a = a;
				best_p = p;
			}
		}
	} while (next_permutation(p.begin(), p.end()));
	best_p = sort_points(best_p);
	for (int i = l; i <= r; ++i) v[i] = best_p[i - l];
}

void MinimumAreaPolygonization::preprocess(vector<pdd> & v) {
	sort(v.begin(), v.end());

	if (v.size() < 3) return;

	vector<pdd> s = { v[0], v[1] };
	for (int i = 2; i < v.size(); ++i) {
		while (s.size() > 1) {
			int n = s.size();
			pdd a = { s[n - 1].first - s[n - 2].first, s[n - 1].second - s[n - 2].second };
			pdd b = { v[i].first - s[n - 1].first, v[i].second - s[n - 1].second };
			if (v[i] == s.back() || abs(cos_angle(a, b) - 1) < eps) {
				s.pop_back();
			} else {
				break;
			}
		}
		s.push_back(v[i]);
	}

	v = s;
}

vector<pdd> MinimumAreaPolygonization::solve(vector<pdd> & v, int l, int r) {
	if (r - l + 1 <= 5) {
		brute_optimal_polygon(v, l, r);
		vector<pdd> poly;
		for (int i = l; i <= r; ++i)poly.push_back(v[i]);
		auto hull = brute_hull(poly);
		return hull;
	} else {
		int mid = (r + l) / 2;
		auto lhull = solve(v, l, mid);
		auto rhull = solve(v, mid + 1, r);
		return merge_polygons(v, l, r, mid, lhull, rhull);
	}
}

inline double MinimumAreaPolygonization::det(double a, double b, double c, double d) {
	return a * d - b * c;
}
