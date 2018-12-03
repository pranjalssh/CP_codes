/*
	できる限り整数でやること！
	誤差に強い操作(面積など)のみ検証
	！！誤差に弱い操作(intersect後の点の数など)は注意！！
*/

typedef complex<double> P;
typedef vector<P> G;
double cross(const P& a, const P& b) {
	return imag(conj(a)*b);
}
double dot(const P& a, const P& b) {
	return real(conj(a)*b);
}

#define EQ(x, y) (abs((x) - (y)) < EPS)
#define LE(x, y) ((x) <= (y) + EPS)
bool intersect_1pt(const P& a, const P& b,
                   const P& c, const P& d, P &r) {
  double D =  cross(b - a, d - c);
  if (EQ(D, 0)) return false;
  double t =  cross(c - a, d - c) / D;
  double s = -cross(a - c, b - a) / D;
  r = a + t * (b - a);
  return LE(0, t) && LE(t, 1) && LE(0, s) && LE(s, 1);
}

enum { OUT, ON, IN };
int convex_contains(const G &O, const P &p) {
	const int n = O.size();
	P g = (O[0] + O[n/3] + O[2*n/3]) / 3.0; // inner-point
	int a = 0, b = n;
	while (a+1 < b) { // invariant: c is in fan g-P[a]-P[b]
		int c = (a + b) / 2;
		if (cross(O[a]-g, O[c]-g) > 0) { // angle < 180 deg
			if (cross(O[a]-g, p-g) > 0 && cross(O[c]-g, p-g) < 0) b = c;
			else												  a = c;
		} else {
			if (cross(O[a]-g, p-g) < 0 && cross(O[c]-g, p-g) > 0) a = c;
			else												  b = c;
		}
	}
	b %= n;
	if (cross(O[a] - p, O[b] - p) < 0) return OUT;
	if (cross(O[a] - p, O[b] - p) > 0) return IN;
	return ON;
}


G convex_intersect(const G &O, const G &Q) {
	const int n = O.size(), m = Q.size();
	int a = 0, b = 0, aa = 0, ba = 0;
	enum { Pin, Qin, Unknown } in = Unknown;
	G R;
	do {
		int a1 = (a+n-1) % n, b1 = (b+m-1) % m;
		double C = cross(O[a] - O[a1], Q[b] - Q[b1]);
		double A = cross(O[a1] - Q[b], O[a] - Q[b]);
		double B = cross(Q[b1] - O[a], Q[b] - O[a]);
		P r;
		if (intersect_1pt(O[a1], O[a], Q[b1], Q[b], r)) {
			if (in == Unknown) aa = ba = 0;
			R.push_back( r );
			in = B > 0 ? Pin : A > 0 ? Qin : in;
		}
		if (C == 0 && B == 0 && A == 0) {
			if (in == Pin) { b = (b + 1) % m; ++ba; }
			else					 { a = (a + 1) % m; ++aa; }
		} else if (C >= 0) {
			if (A > 0) { if (in == Pin) R.push_back(O[a]); a = (a+1)%n; ++aa; }
			else	   { if (in == Qin) R.push_back(Q[b]); b = (b+1)%m; ++ba; }
		} else {
			if (B > 0) { if (in == Qin) R.push_back(Q[b]); b = (b+1)%m; ++ba; }
			else	   { if (in == Pin) R.push_back(O[a]); a = (a+1)%n; ++aa; }
		}
	} while ( (aa < n || ba < m) && aa < 2*n && ba < 2*m );
	if (in == Unknown) {
		if (convex_contains(Q, O[0])) return O;
		if (convex_contains(O, Q[0])) return Q;
	}
	return R;
}

int ccw(P a, P b, P c) {
	b -= a; c -= a;
	if(cross(b, c) > 0)   return +1;       // counter clockwise
	if(cross(b, c) < 0)   return -1;       // clockwise
	if(dot(b, c) < 0)     return +2;       // c--a--b on line
	if(norm(b) < norm(c)) return -2;       // a--b--c on line
	return 0;
}

P crosspoint(P a, P b, P c, P d) {
	double A = cross(b - a, d - c), B = cross(b - a, b - c);
	if(abs(A) < EPS && abs(B) < EPS) return c;
	return c + B / A * (d - c);
}

G convex_cut(const G &poly, const P &a, const P &b) {
	G r;
	rep(i, poly.size()) {
		P p = poly[i], q = poly[(i+1) % poly.size()];
		int c = ccw(a, b, p), d = ccw(a, b, q);
		if(c != -1) r.push_back(p);
		if(c * d < 0) r.push_back(crosspoint(p, q, a, b));
	}
	return r;
}

double area(const G& poly) {
	double A = 0;
	rep(i, poly.size())
		A += cross(poly[i], poly[(i+1)%poly.size()]);
	return abs(A) / 2;
}
