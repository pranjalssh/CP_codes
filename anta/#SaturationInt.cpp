struct SaturationInt {
	typedef long long T; typedef double Float;
	static T Max;
	T x;
	static inline T sat(T x) { return x > Max ? Max : x; }
	SaturationInt() { }
	SaturationInt(T x_): x(sat(x_)) { }
	T get() const { return x; }

	SaturationInt operator+(const SaturationInt &that) const {
		return SaturationInt(x + that.x);
	}
	SaturationInt operator*(const SaturationInt &that) const {
		return SaturationInt((Float)x * that.x > Max ? Max : x * that.x);
	}
	SaturationInt &operator+=(const SaturationInt &that) { return *this = *this + that; }
	SaturationInt &operator*=(const SaturationInt &that) { return *this = *this * that; }
};
SaturationInt::T SaturationInt::Max = INFL;
