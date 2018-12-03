
struct DoubleInt {
	typedef DoubleInt This;
	typedef unsigned long long T;
	typedef unsigned int Half;
	static const int Bits = sizeof(T) * 8, HalfBits = sizeof(Half) * 8;
	T a, b;
	DoubleInt() { a = b = 0; }
	DoubleInt(T x) { a = x; b = 0; }
	This& operator<<=(int y) {
		if(y < Bits) {
			b <<= y;
			if(y) b |= a >> (Bits - y);
			a <<= y;
		} else {
			b = a << (y - Bits);
			a = 0;
		}
		return *this;
	}
	This operator<<(int y) { return *this <<= y; }
	This& operator+=(const This& y) {
		T aa = a + y.a;
		b += y.b + (a > aa);
		a = aa;
		return *this;
	}
	inline const Half& aa() const { return *(Half*)&a; }
	inline const Half& ab() const { return *((Half*)&a+1); }
	inline const Half& ba() const { return *(Half*)&b; }
	inline const Half& bb() const { return *((Half*)&b+1); }
	This operator*(const This& y) {
		This t;
		t += This((T)aa() * y.aa());
		t += This((T)aa() * y.ab()) << HalfBits;
		t += This((T)aa() * y.ba()) << HalfBits*2;
		t += This((T)aa() * y.bb()) << HalfBits*3;
		t += This((T)ab() * y.aa()) << HalfBits;
		t += This((T)ab() * y.ab()) << HalfBits*2;
		t += This((T)ab() * y.ba()) << HalfBits*3;
		t += This((T)ba() * y.aa()) << HalfBits*2;
		t += This((T)ba() * y.ab()) << HalfBits*3;
		t += This((T)bb() * y.aa()) << HalfBits*3;
		return t;
	}
	This& operator*=(const This& y) {
		return *this = *this * y;
	}
	pair<This, This> divMod(const This& y) {
		
	}
};
