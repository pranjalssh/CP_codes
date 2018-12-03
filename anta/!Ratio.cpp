//オーバーフローに注意
//比較だけでも掛け算するので！
template<typename T>T gcd(T x, T y) { return y == 0 ? x : gcd(y,x%y); }

struct Ratio {
	typedef ll T;
	T x, y;
	Ratio(): x(0), y(1) { }
	Ratio(T x_): x(x_), y(1) { }
	Ratio(T x_, T y_): x(x_), y(y_) { normalize(); }
	double toDouble() { return double(x) / y; }
	void normalize() {
		T g = gcd(abs(x), abs(y));
        if(g == 0) return;
		x /= g; y /= g;
		if(y < 0) x = -x, y = -y;
		if(x == 0) y = 1;
	}
	bool operator==(const Ratio& q) const { return x == q.x && y == q.y; }
	bool operator!=(const Ratio& q) const { return x != q.x || y != q.y; }
	bool operator<(const Ratio& q) const { return x*q.y < y*q.x; }
	bool operator<=(const Ratio& q) const { return x*q.y <= y*q.x; }
	bool operator>(const Ratio& q) const { return x*q.y > y*q.x; }
	bool operator>=(const Ratio& q) const { return x*q.y >= y*q.x; }
	Ratio& operator+=(const Ratio& q) {
		T g = gcd(y,q.y); x = q.y/g*x + y/g*q.x, y = y/g*q.y; normalize();
		return *this;
	}
	Ratio& operator-=(const Ratio& q) {
		T g = gcd(y,q.y); x = q.y/g*x - y/g*q.x, y = y/g*q.y; normalize();
		return *this;
	}
	Ratio& operator*=(const Ratio& q) {
		x = x*q.x, y = y*q.y; normalize();
		return *this;
	}
	Ratio& operator/=(const Ratio& q) {
		x = x*q.y, y = y*q.x; normalize();
		return *this;
	}
	Ratio operator+(const Ratio& q) const { return Ratio(*this) += q; }
	Ratio operator-(const Ratio& q) const { return Ratio(*this) -= q; }
	Ratio operator*(const Ratio& q) const { return Ratio(*this) *= q; }
	Ratio operator/(const Ratio& q) const { return Ratio(*this) /= q; }
	Ratio operator-() const { return Ratio(-x, y); }
};
ostream& operator<<(ostream &o, const Ratio& p) { o << p.x << "/" << p.y; return o; }
