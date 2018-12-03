struct Mat22 {
	typedef GModInt T;
	T a, b, c, d;
	Mat22(): a(0), b(0), c(0), d(0) { }
	Mat22(T a_, T b_, T c_, T d_): a(a_), b(b_), c(c_), d(d_) { }
	static Mat22 identity() { return Mat22(1, 0, 0, 1); }

	Mat22 operator*(const Mat22 &that) const {
		return Mat22
			( a * that.a + b * that.c, a * that.b + b * that.d
			, c * that.a + d * that.c, c * that.b + d * that.d);
	}
	Mat22 &operator*=(const Mat22 &that) { return *this = *this * that; }
};
