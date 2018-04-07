#include "JoeSimpson.h"



JoeSimpson::JoeSimpson(vector<pdd> & poly){
	this->poly = poly;
	this->original_points = poly;
	this->is_solved = false;
}


vector<pdd> JoeSimpson::run() {
	if (is_solved) return stack;

	prepare();
	init_alpha();

	int t = 0;

	vector<JoeSimpsonPoint> v;
	vector<JoeSimpsonPoint> s;

	for (auto & p : poly) v.push_back(JoeSimpsonPoint(p, true));

	visibility_polygon(v, s, t);

	for (auto & jsp : s) if (jsp.is_original) stack.push_back(jsp.point);

	rotate_polygon(stack, 2 * PI + rotation_angle);

	for (auto & p : stack) p.first += delta.first, p.second += delta.second;
	reverse(stack.begin() + 1, stack.end());

	is_solved = true;
	return stack;
}


void JoeSimpson::prepare() {
	auto tmp1 = poly;

	delta = poly[0];
	for (int i = 0; i < poly.size(); ++i) {
		tmp1[i] = { tmp1[i].first - poly[0].first, tmp1[i].second - poly[0].second };
	}

	auto tmp2 = tmp1;

	double angle2 = angle({ poly[0].first + INF, poly[0].second }, poly[0], poly.back());

	rotate_polygon(tmp2, angle2);

	reverse(tmp2.begin() + 1, tmp2.end());

	if (isCCW(tmp2)) {
		poly = tmp2;
		rotation_angle = angle2;
		return;
	}
	
	double angle1 = angle({ poly[0].first + INF, poly[0].second }, poly[0], poly[1]);

	rotate_polygon(tmp1, angle1);

	if (isCCW(tmp1)) {
		poly = tmp1;
		rotation_angle = angle1;
		return;
	}
}


void JoeSimpson::init_alpha() {
	for (int i = 1; i < poly.size(); ++i) {
		alpha.push_back(i == 1 ? 0.0 : alpha.back() + angle(poly[i - 1], poly[0], poly[i]));
	}
}


void JoeSimpson::rotate_point(pdd & p, double angle) {
	double si = sin(angle);
	double co = cos(angle);
	p = { p.first * co + p.second * si , p.first * si - p.second * co };
}


void JoeSimpson::rotate_polygon(vector<pdd> & poly, double angle) {
	for (pdd & p : poly) rotate_point(p, angle);
}


bool JoeSimpson::isCCW(const vector<pdd> & poly) {
	double ck = 0;
	for (int i = 0; i < poly.size(); ++i) {
		int nx = (i + 1) % poly.size();
		ck += (poly[nx].first - poly[i].first) * (poly[nx].second + poly[i].second);
	}
	return ck <= 0;
}


pdd JoeSimpson::from_polar(double r, double theta) {
	double x = r * cos(theta);
	double y = r * sin(theta);
	return { x , y };
}


pdd JoeSimpson::to_polar(pdd p) {
	double r = sqrt(p.first * p.first + p.second * p.second);
	double theta = atan(p.second / p.first);
	if (abs(theta) < 1e-10) theta = 0.0;
	while (theta < 0) theta += PI;
	return { r , theta };
}


double JoeSimpson::angle(const pdd & a, const pdd & b, const pdd & c) {
	pdd oa = { b.first - a.first, b.second - a.second };
	pdd ob = { b.first - c.first, b.second - c.second };
	return atan2(oa.first * ob.second - oa.second * ob.first, oa.first * ob.first + oa.second * ob.second);
}


void JoeSimpson::visibility_polygon(vector<JoeSimpsonPoint> & v, vector<JoeSimpsonPoint> & s, int & t) { 
	// visible vertexies of poly from poly[0]

	s.push_back(v[0]);
	s.push_back(v[1]);

	string vpcase = "";
	int i = 0;
	t = 0;
	vector<double> a{ 0, alpha[1] };
	pdd w;
	bool ccw;

	if (alpha[2] >= alpha[1]) {
		vpcase = "advance";
	}
	else {
		vpcase = "scan";
		ccw = true;
		w = from_polar(INF, to_polar(v[1].point).second);
	}

	while (vpcase != "finish")
	{
		if (vpcase == "advance") {
			advance(v, s, a, t, i, vpcase, ccw, w);
		}
		else if (vpcase == "retard") {
			retard(v, s, a, t, i, vpcase, ccw, w);
		}
		else if (vpcase == "scan") {
			scan(v, s, a, t, i, vpcase, ccw, w);
		}
	}
}


void JoeSimpson::advance(vector<JoeSimpsonPoint> & v, vector<JoeSimpsonPoint> & s, vector<double> & a,
	int & t, int & i, string & vpcase, bool & ccw, pdd & w) {
	qDebug() << "IN ADVANCE\n";
	while (vpcase == "advance") {
		if (alpha[i + 2] <= 2 * PI) {
			++i;
			++t;
			s.push_back(v[i + 1]);
			a.push_back(alpha[i + 1]);
			if (i + 2 >= v.size()) {
				vpcase = "finish";
				return;
			}

			int or = orientation_(v[i].point, v[i + 1].point, v[i + 2].point);
			if (or == 1 && alpha[i + 2] < alpha[i + 1]) { // right
				vpcase = "scan";
				ccw = true;
				w = from_polar(INF, to_polar(v[i + 1].point).second);
			}
			else if (alpha[i + 2] < alpha[i + 1] && or == -1) {
				vpcase = "retard";
			}
		}
		else {
			if (a.back() < 2 * PI) {
				++t;
				pdd A = v[i + 1].point;
				pdd B = v[i + 2].point;
				pdd C = v[0].point;
				pdd D = v[1].point;

				pdd E = lines_intersect(A, B, C, D);
				a.push_back(a.back() + angle(s.back().point, v[0].point, E));
				s.push_back(JoeSimpsonPoint(E, false));
			}
			vpcase = "scan";
			ccw = false;
			w = v[1].point;
		}
	}
}


void JoeSimpson::retard(vector<JoeSimpsonPoint> & v, vector<JoeSimpsonPoint> & s, vector<double> & a,
	int & t, int & i, string & vpcase, bool & ccw, pdd & w) {
	//qDebug() << "IN RETARD\n";
	while (vpcase == "retard") {
		int j = s.size() - 2;
		bool case_a, case_b;
		while (true) {
			case_a = a[j] < alpha[i + 2] && alpha[i + 2] <= a[j + 1];
			case_b = alpha[i + 2] <= a[j] && abs(a[j] - a[j + 1]) < 1e-7 
				&& intersect(v[i + 1].point, v[i + 2].point, s[j].point, s[j + 1].point);
			if (case_a || case_b) break;
			--j;
			//if (j < 0)qDebug() << "Found :(\n";
		}
		pdd last;
		while (s[j] != s.back()) {
			last = s.back().point;
			s.pop_back();
			a.pop_back();
		}
		if (case_a) {
			++i;
			t = j + 1;
			pdd A = last;
			pdd B = s.back().point;
			pdd C = v[0].point;
			pdd D = v[i + 1].point;

			pdd E = lines_intersect(A, B, C, D);

			a.push_back(a.back() + angle(s.back().point, v[0].point, E));
			s.push_back(JoeSimpsonPoint(E, false));
			++t;
			s.push_back(v[i + 1]);
			a.push_back(alpha[i + 1]);
			if (i + 2 >= v.size()) {
				vpcase = "finish";
				return;
			}
			int or = orientation_(v[i].point, v[i + 1].point, v[i + 2].point);
			if (alpha[i + 2] >= alpha[i + 1] && or == 1) {
				vpcase = "advance";
			}
			else if (alpha[i + 2] > alpha[i + 1] && or == -1) {
				vpcase = "scan";
				ccw = false;
				w = v[i + 1].point;
				--t;
				s.pop_back();
				a.pop_back();
			}
			else {
				--t;
				s.pop_back();
				a.pop_back();
			}
		}
		else {
			if (abs(alpha[i + 2] - a.back()) < 1e-10 && alpha[i + 3] > alpha[i + 2] &&
				orientation_(v[i + 1].point, v[i + 2].point, v[i + 3].point) == 1) {
				vpcase = "advance";
				++i;
				t = j + 1;
				s.push_back(v[i + 1]);
				a.push_back(alpha[i + 1]);
			}
			else {
				vpcase = "scan";
				t = j;
				ccw = true;
				pdd A = v[i + 1].point;
				pdd B = v[i + 2].point;
				pdd C = s.back().point;
				pdd D = last;

				w = lines_intersect(A, B, C, D);
			}
		}
	}
}


void JoeSimpson::scan(vector<JoeSimpsonPoint> & v, vector<JoeSimpsonPoint> & s, vector<double> & a,
	int & t, int & i, string & vpcase, bool & ccw, pdd & w) {
	qDebug() << "IN SCAN\n";
	while (vpcase == "scan")
	{
		++i;
		bool aaa1 = alpha[i + 2] > a.back();
		bool aaa2 = a.back() >= alpha[i + 1];
		if (ccw && alpha[i + 2] > a.back() && a.back() >= alpha[i + 1]) {
			if (intersect_with_w(v[i + 1].point, v[i + 2].point, s.back().point, w)) {
				pdd A = v[i + 1].point;
				pdd B = v[i + 2].point;
				pdd C = s.back().point;

				pdd E = lines_intersect(A, B, C, w);

				a.push_back(a.back() + angle(s.back().point, v[0].point, E));
				s.push_back(JoeSimpsonPoint(E, false));
				++t;
				vpcase = "advance";
			}
		}
		else if (ccw == false && alpha[i + 2] <= a.back() && a.back() < alpha[i + 1]) {
			if (/*intersect*/intersect_with_w(v[i + 1].point, v[i + 2].point, s.back().point, w)) {
				vpcase = "retard";
			}
		}
	}
}


bool JoeSimpson::intersect_with_w(pdd & A, pdd & B, pdd & C, pdd & w) {
	if (intersect(A, B, C, w)) return 1;
	pdd W = { -w.first, -w.second };
	if (intersect(A, B, C, W)) {
		w = W;
		return 1;
	}
	return 0;
}


JoeSimpson::~JoeSimpson()
{
}
