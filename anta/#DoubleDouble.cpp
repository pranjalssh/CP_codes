//基本的な関数以外のsinとかPI定数でさえも精度が数bit落ちてる…
//あと遅いし無限ループもするかもね
//てかなんか精度悪すぎてバイアスが超かかってて使えねー

#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <set>
#include <map>
#include <queue>
#include <iostream>
#include <sstream>
#include <cstdio>
#include <cmath>
#include <ctime>
#include <cstring>
#include <cctype>
#include <list>
#include <cassert>
#include <complex>
#define rep(i,n) for(int (i)=0;(i)<(int)(n);++(i))
#define rer(i,l,u) for(int (i)=(int)(l);(i)<=(int)(u);++(i))
#define reu(i,l,u) for(int (i)=(int)(l);(i)<(int)(u);++(i))
#if defined(_MSC_VER) || __cplusplus > 199711L
#define aut(r,v) auto r = (v)
#else
#define aut(r,v) typeof(v) r = (v)
#endif
#define each(it,o) for(aut(it, (o).begin()); it != (o).end(); ++ it)
#define all(o) (o).begin(), (o).end()
#define pb(x) push_back(x)
#define mp(x,y) make_pair((x),(y))
#define mset(m,v) memset(m,v,sizeof(m))
#define INF 0x3f3f3f3f
#define INFL 0x3f3f3f3f3f3f3f3fLL
#define EPS 1e-9
using namespace std;
typedef vector<int> vi; typedef pair<int,int> pii; typedef vector<pair<int,int> > vpii;
typedef long long ll; typedef vector<long long> vl; typedef pair<long long,long long> pll; typedef vector<pair<long long,long long> > vpll;
typedef vector<string> vs; typedef long double ld;


template<typename NumInfo>
struct DoubleNum {
	typedef typename NumInfo::Num Num;
	typedef DoubleNum This;
	Num hi, lo;
	DoubleNum() { set(0); }
	DoubleNum(Num x) { set(x); }
	DoubleNum(int x) { set(Num(x)); }
	DoubleNum(Num x, Num y) { set(x, y); }
	static This nan() { Num t = 0.; return DoubleNum(t/0.,t/0.); }
	void set(Num a, Num b = Num()) {
		hi = a; lo = b;
	}
	This &operator+=(const This &that) {
		Num S, T, e, f, s, t, H, h;
		S = hi + that.hi, T = lo + that.lo;
		e = S - hi, f = T - lo;
		s = S - e, t = T - f;
		s = (that.hi - e) + (hi - s), t = (that.lo - f) + (lo - t);
		e = s + T, H = S + e, h = e + (S - H), e = t + h;
		hi = H + e, lo = e + (H - hi);
		return *this;
	}
	This operator+(const This &that) const { return This(*this) += that; }
	This &operator+=(const Num &that) { return *this += This(that); }
	This operator+(const Num &that) const { return This(*this) += that; }
	This &operator-=(const This &that) { return *this += -that; }
	This operator-(const This &that) const { return This(*this) += -that; }
	This &operator-=(const Num &that) { return *this -= This(that); }
	This operator-(const Num &that) const { return This(*this) -= that; }
	This operator-() const { return This(-hi, -lo); }
	This &operator*=(const This &that) {
		Num hx, tx, hy, ty, C, c;
		split(hi, hx, tx); split(that.hi, hy, ty);
		C = hi*that.hi;
		c = ((((hx*hy-C)+hx*ty)+tx*hy)+tx*ty)+(hi*that.lo+lo*that.hi);
		Num zhi = C+c; hx = C-zhi; 
		Num zlo = c+hx;
		hi = zhi, lo = zlo;
		return *this;
	}
	This operator*(const This &that) const { return This(*this) *= that; }
	This &operator*=(const Num &that) { return *this *= This(that); }
	This operator*(const Num &that) const { return This(*this) *= that; }
	This &operator/=(const This &that) {
		Num hc, tc, hy, ty, C, c, U, u;
		C = hi/that.hi;
		split(C, hc, tc);
		split(that.hi, hy, ty);
		U = C * that.hi; 
		u = (((hc*hy-U)+hc*ty)+tc*hy)+tc*ty;
		c = ((((hi-U)-u)+lo)-C*that.lo)/that.hi;
		u = C+c; 
		Num zhi = u;
		Num zlo = (C-u)+c;
		hi = zhi, lo = zlo;
		return *this;
	}
	This operator/(const This &that) const { return This(*this) /= that; }
	This &operator/=(const Num &that) { return *this /= This(that); }
	This operator/(const Num &that) const { return This(*this) /= that; }
	This recip() const {
		Num hc, tc, hy, ty, C, c, U, u;
		C = Num(1) / hi;
		split(C, hc, tc);
		split(hi, hy, ty);
		U = C * hi;
		u = hc * hy - U + hc * ty + tc * hy + tc * ty;
		c = (Num(1) - U - u - C * lo) / hi;
		Num zhi = C + c, zlo = (C - zhi) + c;
		return This(zhi, zlo);
	}
	bool is_zero() const { return hi == 0 && lo == 0; }
	bool is_positive() const { return hi > 0 || (hi == 0 && lo > 0); }
	bool is_negative() const { return hi < 0 || (hi == 0 && lo < 0); }
	bool is_nan() const { return hi != hi || lo != lo; }
	int signum() const { return is_positive() ? 1 : is_negative() ? -1 : 0; }

	bool operator==(const This &that) const { return hi == that.hi && lo == that.lo; }
	bool operator!=(const This &that) const { return !(*this == that); }
	bool operator<(const This &that) const { return hi < that.hi || (hi == that.hi && lo < that.lo); }
	bool operator>(const This &that) const { return that < *this; }
	bool operator<=(const This &that) const { return !(*this > that); }
	bool operator>=(const This &that) const { return !(*this < that); }

	ostream &dump(ostream &o) const {
		std::streamsize p = o.precision(NumInfo::PRECISION_IN_BASE10() * 2);
		o << "(" << hi << "," << lo << ")";
		o.precision(p);
		return o;
	}
	static int magnitude(Num x) {
		Num xabs = abs(x);
		int xmag = NumInfo::getInt(floor(log(xabs) / log(Num(10))));
		if(pow(Num(10), xmag + 1) <= xabs) xmag ++;
		return xmag;
	}
	string to_string(int printDigs = -1) const {
		if(printDigs == -1) printDigs = NumInfo::PRECISION_IN_BASE10() * 2;
		return truncateTraidingZeros(to_standard_notation(printDigs));
	}
	string to_standard_notation(int printDigs) const {
		if(is_zero()) return "0.0";
		if(is_nan()) return "NaN";
		int mag;
		string digs = extract_significant_digits(true, mag, printDigs);
		int decimalPos = mag + 1;
		string num = digs;
		if(digs[0] == '.')
			num = string("0") + digs;
		else if(decimalPos < 0)
			num = string("0.") + string(-decimalPos, '0') + digs;
		else if(digs.find('.') == string::npos) {
			int numZeros = decimalPos - digs.size();
			string zeros = string(numZeros, '0');
			num = digs + zeros + ".0";
		}
		if(is_negative()) return string("-") + num;
		else return num;
	}


	This abs_() const { return is_negative() ? -*this : *this; }
	This floor_() const {
		Num fhi = floor(hi), flo = 0;
		if(fhi == hi) flo = floor(lo);
		return This(fhi, flo);
	}
	This mod_(const This &that) const {
		return *this - (*this / that).floor_() * that;
	}
	This sqrt_() const {
		Num x = Num(1) / sqrt(hi);
		Num ax = hi * x;
		This axdd = This(ax);
		This diff = *this - axdd * axdd;
		Num d2 = diff.hi * (x / Num(2));
		return axdd + d2;
	}
	This exp_() const {
		This x = 1, t = -1, u = 0;
		for(int i = 0; t != u; i ++) {
			t = u;
			u += x;
			x *= *this;
			x /= Num(i+1);
		}
		return t;
	}
	This log_() const {
		This t = -1, u = 1, diff = 0, pdiff = 3;
		for(int i = 0; t != u; i ++) {
			diff = -*this * (-u).exp_() + Num(1);
			if(abs(pdiff) <= abs(diff)) break;
			pdiff = diff;
			t = u;
			u = u - diff;
		}
		return t;
	}
	This pow_(int x) const {
		if(x < 0) return pow_(-x).recip();
		This t = 1, u = *this;
		int y = abs(x);
		while(y) {
			if(y & 1) t *= u;
			u *= u;
			y >>= 1;
		}
		return t;
	}
	This pow_(const This &that) const {
		return (that * log_()).exp_();
	}
	This sin_() const {
		This x = 1, t = -1, u = 0;
		for(int i = 0; t != u; i ++) {
			if(i % 2 == 1) {
				t = u;
				if(i % 4 == 1) u += x;
				else u -= x;
			}
			x *= *this;
			x /= Num(i+1);
		}
		return t;
	}
	This cos_() const {
		This x = 1, t = -1, u = 0;
		for(int i = 0; t != u; i ++) {
			if(i % 2 == 0) {
				t = u;
				if(i % 4 == 0) u += x;
				else u -= x;
			}
			x *= *this;
			x /= Num(i+1);
		}
		return t;
	}
	This tan_() const {
		return sin_() / cos_();
	}
	This asin_() const {
		This t = -1, u = 1, diff = 0, pdiff = 3;
		for(int i = 0; t != u; i ++) {
			diff = (u.sin_() - *this) / u.cos_();
			if(abs(pdiff) <= abs(diff)) break;
			pdiff = diff;
			t = u;
			u = u - diff;
		}
		return t;
	}
	This acos_() const {
		return PI()/Num(2) - asin_();
	}
	This atan_() const {
		if(is_negative()) return -(-*this).atan_();
		if(*this > This(1)) return PI() / Num(2) - recip().atan_();
		This t = -1, u = 1, diff = 0, pdiff = 3;
		for(int i = 0; t != u; i ++) {
			This s = u.sin_(), c = u.cos_();
			diff = c * (s - *this * c);
			if(abs(pdiff) <= abs(diff)) break;
			pdiff = diff;
			t = u;
			u = u - diff;
		}
		return t;
	}
	This atan2_(const This &x) const {
		if(x.is_zero()) return PI() / Num(2) * Num(signum());
		return (*this / x).atan_() + (!x.is_negative() ? 0 : is_negative() ? -PI() : PI());
	}
	This hypot_(const This &that) const {
		return (*this * *this + that * that).sqrt_();
	}
	static This PI_cache; static bool PI_cached;
	static This PI() {
		if(PI_cached) return PI_cache;
		else return PI_cache = This(1).atan_() * Num(4);
	}

private:
	static void split(Num a, Num &ahi, Num &alo) {
		NumInfo::split(a, ahi, alo);
	}
	string extract_significant_digits(bool insertDecimalPoint, int &mag, int printDigs) const {
		This BASE = This(10);
		This y = abs_();
		mag = magnitude(y.hi);
		This scale = BASE.pow_(mag);
		y /= scale;
		if(y > BASE) {
			y /= BASE;
			mag ++;
		}else if(y < This(1)) {
			y *= BASE;
			mag --;
		}
		int decimalPos = mag + 1;
		string buf;
		int numDigs = printDigs;
		for(int i = 0; i <= numDigs; i ++) {
			if(insertDecimalPoint && i == decimalPos)
				buf += '.';
			int digit = NumInfo::getInt(y.hi);
			if(digit < 0) break;
			bool rebiasBy10 = false;
			char digitChar = 0;
			if(digit > 9) {
				rebiasBy10 = true;
				digitChar = '9';
			}else digitChar = (char)('0' + digit);
			buf += digitChar;
			y = (y - Num(digit)) * BASE;
			if(rebiasBy10) y += BASE;
			bool continueExtractingDigits = true;
			int remMag = magnitude(y.hi);
			if(remMag < 0 && std::abs(remMag) >= numDigs - i)
				continueExtractingDigits = false;
			if(!continueExtractingDigits)
				break;
		}
		return buf;
	}
	static string truncateTraidingZeros(string x) {
		int i = x.size();
		while(i >= 2 && x[i-1] == '0' && x[i-2] == '0')
			i --;
		return x.substr(0, i);
	}
};
template<typename NumInfo> DoubleNum<NumInfo> DoubleNum<NumInfo>::PI_cache;
template<typename NumInfo> bool DoubleNum<NumInfo>::PI_cached = false;
template<typename NumInfo>
ostream &operator<<(ostream &o, const DoubleNum<NumInfo> &t) {
	return o << t.to_string();
}
template<typename NumInfo> DoubleNum<NumInfo> abs(const DoubleNum<NumInfo> &x) { return x.abs_(); }
template<typename NumInfo> DoubleNum<NumInfo> floor(const DoubleNum<NumInfo> &x) { return x.floor_(); }
template<typename NumInfo> DoubleNum<NumInfo> sqrt(const DoubleNum<NumInfo> &x) { return x.sqrt_(); }
template<typename NumInfo> DoubleNum<NumInfo> exp(const DoubleNum<NumInfo> &x) { return x.exp_(); }
template<typename NumInfo> DoubleNum<NumInfo> log(const DoubleNum<NumInfo> &x) { return x.log_(); }
template<typename NumInfo> DoubleNum<NumInfo> pow(const DoubleNum<NumInfo> &x, int y) { return x.pow_(y); }
template<typename NumInfo> DoubleNum<NumInfo> pow(const DoubleNum<NumInfo> &x, const DoubleNum<NumInfo> &y) { return x.pow_(y); }
template<typename NumInfo> DoubleNum<NumInfo> sin(const DoubleNum<NumInfo> &x) { return x.sin_(); }
template<typename NumInfo> DoubleNum<NumInfo> cos(const DoubleNum<NumInfo> &x) { return x.cos_(); }
template<typename NumInfo> DoubleNum<NumInfo> tan(const DoubleNum<NumInfo> &x) { return x.tan_(); }
template<typename NumInfo> DoubleNum<NumInfo> asin(const DoubleNum<NumInfo> &x) { return x.asin_(); }
template<typename NumInfo> DoubleNum<NumInfo> acos(const DoubleNum<NumInfo> &x) { return x.acos_(); }
template<typename NumInfo> DoubleNum<NumInfo> atan(const DoubleNum<NumInfo> &x) { return x.atan_(); }
template<typename NumInfo> DoubleNum<NumInfo> atan2(const DoubleNum<NumInfo> &x, const DoubleNum<NumInfo> &y) { return x.atan2_(y); }
template<typename NumInfo> DoubleNum<NumInfo> hypot(const DoubleNum<NumInfo> &x, const DoubleNum<NumInfo> &y) { return x.hypot_(y); }

struct NumInfoDouble {
	typedef double Num;
	static void split(Num a, Num &ahi, Num &alo) {
		Num t = a * 134217729.;	//2^27+1
		ahi = t - (t - a);
		alo = a - ahi;
	}
	static int getInt(Num x) { return (int)x; }
	static int PRECISION_IN_BASE10() { return 16; }
};
typedef DoubleNum<NumInfoDouble> DoubleDouble;

struct NumInfoLongDouble {
	typedef long double Num;
	static void split(Num a, Num &ahi, Num &alo) {
		Num t = a * 4294967297.L;	//2^32+1
		ahi = t - (t - a);
		alo = a - ahi;
	}
	static int getInt(Num x) { return (int)x; }
	static int PRECISION_IN_BASE10() { return 20; }
};
template<typename NumInfo>
struct NumInfoDoubleDouble {
	typedef DoubleNum<NumInfo> Num;
	static void split(Num a, Num &ahi, Num &alo) {
		ahi = a.hi;
		alo = a.lo;
	}
	static int getInt(Num x) { return NumInfo::getInt(x.hi); }
	static int PRECISION_IN_BASE10() { return NumInfo::PRECISION_IN_BASE10() * 2; }
};
typedef DoubleNum<NumInfoDouble> DoubleDouble;
typedef DoubleNum<NumInfoLongDouble> DoubleLongDouble;
//QuadDoubleで割り算は精度出てるように見えるのにexpとかは精度出ない(1e-32とか)…
typedef DoubleNum<NumInfoDoubleDouble<NumInfoDouble> > QuadDouble;
typedef DoubleNum<NumInfoDoubleDouble<NumInfoDoubleDouble<NumInfoDouble> > > DoubleQuadDouble;

int main() {
	typedef DoubleDouble T;
	T x(2), y(3), z = T(1) / T(2);
	cout << x << " + " << y << " = " << x + y << endl;
	cout << x << " - " << y << " = " << x - y << endl;
	cout << x << " * " << y << " = " << x * y << endl;
	cout << x << " / " << y << " = " << x / y << endl;
	cout << "1 / " << y << " = " << y.recip() << endl;
	cout << "sqrt(" << x << ") = " << x.sqrt_() << endl;
	cout << "exp(" << x << ") = " << x.exp_() << endl;
	cout << "pow(" << x << ", " << y << ") = " << x.pow_(y) << endl;
	cout << "log(" << x << ") = " << x.log_() << endl;
	cout << "PI = " << x.PI() << endl;
	cout << "sin(" << x << ") = " << x.sin_() << endl;
	cout << "cos(" << x << ") = " << x.cos_() << endl;
	cout << "tan(" << x << ") = " << x.tan_() << endl;
	cout << "asin(" << z << ") = " << z.asin_() << endl;
	cout << "acos(" << z << ") = " << z.acos_() << endl;
	cout << "atan(" << z << ") = " << z.atan_() << endl;
	cout << "atan2(" << x << ", " << y << ") = " << x.atan2_(y) << endl;
	cout << "hypot(" << x << ", " << y << ") = " << x.hypot_(y) << endl;
	y = -1, x = 0;
	cout << "atan2(" << y << ", " << x << ") = " << y.atan2_(x) << endl;
	return 0;
}
