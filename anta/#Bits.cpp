	static const int N = (18 * 18 * 2 + 31) / 32 + 2;
	unsigned a[N];
	Bits() { zero(); }
	Bits(int x) { zero(); a[0] = abs(x); if(x < 0) negate(); }
	Bits(unsigned x) { zero(); a[0] = x; }
	Bits(long long x) { zero(); a[0] = (unsigned)abs(x); a[1] = abs(x) >> 32; if(x < 0) negate(); }
	Bits(unsigned long long x) { zero(); a[0] = (unsigned)x; a[1] = x >> 32; }
	explicit operator long long() const {
		if(sign()) return -(long long)-*this;
		else if(largestBitPos() >= 63) return (ll)((1ULL<<63)-1);
		else return a[0] | (ll)a[1] << 32;
	}
	
	Bits &zero(int left = 0, int right = N * 32) {
		unsigned *x = a;
		int l = (left + 31) / 32, r = right / 32;
		if(l <= r) {
			memset(x + l, 0, (r - l) * sizeof(unsigned));
			if(left  % 32 != 0) x[l-1] &=  ((1U << left  % 32) - 1);
			if(right % 32 != 0) x[r]   &= ~((1U << right % 32) - 1);
		}else x[l-1] &= ((1U << left % 32) - 1) | ~((1U << right % 32) - 1);
		return *this;
	}
	inline bool sign() const { return (signed)a[N-1] < 0; }
	
	Bits &operator+=(const Bits &that) {
		unsigned *x = a; const unsigned *y = that.a; bool carry = false;
		for(int i = 0; i < N; i ++) {
			x[i] += y[i] + carry;
			carry = !carry ? x[i] < y[i] : x[i] <= y[i];
		}
		return *this;
	}
	Bits operator+(const Bits &that) const { return Bits(*this) += that; }
	Bits &operator-=(const Bits &that) { return *this += - that; }
	Bits operator-(const Bits &that) const { return Bits(*this) -= that; }
	Bits &negate() { complement(); return ++ *this; }
	Bits operator-() const { return Bits(*this).negate(); }
	Bits &operator++() {
		unsigned *x = a; int i;
		for(i = 0; i < N && ~x[i] == 0; i ++) x[i] = 0;
		if(i != N) ++ x[i];
		return *this;
	}
	Bits &operator--() {
		unsigned *x = a; int i;
		for(i = 0; i < N && x[i] == 0; i ++) x[i] = ~0U;
		if(i != N) -- x[i];
		return *this;
	}
	Bits &complement() { unsigned *x = a; for(int i = 0; i < N; i ++) x[i] = ~x[i]; return *this; }
	Bits operator~() const { return Bits(*this).complement(); }
	Bits &operator<<=(int s) {
		if(s < 0) return *this >>= -s;
		if(s >= N * 32) return zero();
		int d = s / 32, m = s % 32, n = 32 - m; unsigned *x = a;
		if(m == 0) memmove(x + d, x, (N - d) * sizeof(unsigned));
		else {
			for(int i = N-1; i > d; i --) x[i] = x[i-d] << m | x[i-d-1] >> n;
			x[d] = x[0] << m;
		}
		return zero(0, s);
	}
	Bits operator<<(int s) const { return Bits(*this) <<= s; }
	Bits &operator>>=(int s) {
		if(s < 0) return *this <<= -s;
		if(s >= N * 32) return zero();
		int d = s / 32, e = N - d - 1, m = s % 32, n = 32 - m; unsigned *x = a;
		if(m == 0) memmove(x, x + d, (N - d) * sizeof(unsigned));
		else {
			for(int i = 0; i < e; i ++) x[i] = x[i+d] >> m | x[i+d+1] << n;
			x[e] = x[N-1] >> m;
		}
		return zero(N * 32 - s, N * 32);
	}
	Bits operator>>(int s) const { return Bits(*this) >>= s; }
	
	bool operator==(const Bits &that) const { return memcmp(a, that.a, N * sizeof(unsigned)) == 0; }
	bool operator!=(const Bits &that) const { return !(*this == that); }
	bool operator<(const Bits &that) const {
		bool b = sign(), c = that.sign();
		if(b ^ c) return b;
		const unsigned *x = a, *y = that.a;
		for(int i = N-1; i >= 0; i --) if(x[i] != y[i]) return x[i] < y[i];
		return false;
	}
	bool operator>(const Bits &that) const { return that < *this; }
	bool operator<=(const Bits &that) const { return !(*this > that); }
	bool operator>=(const Bits &that) const { return !(*this < that); }
	
	inline bool test(int i) const { return a[i / 32] >> i % 32 & 1; }
	inline Bits &set(int i) { a[i / 32] |= 1U << i % 32; return *this; }
	inline Bits &reset(int i) { a[i / 32] &= ~(1U << i % 32); return *this; }
	int largestBitPos() const {
		const unsigned *x = a; int i;
		for(i = N-1; i >= 0 && x[i] == 0; i --) ;
		if(i == -1) return -1;
		unsigned v = x[i]; int j = i * 32;
		if(v & 0xffff0000U) v >>= 16, j += 16;
		if(v & 0x0000ff00U) v >>= 8, j += 8;
		if(v & 0x000000f0U) v >>= 4, j += 4;
		if(v & 0x0000000cU) v >>= 2, j += 2;
		return j + (v >> 1);
	}
	
	friend ostream &operator<<(ostream &o, Bits t) {
		if(t.sign()) o << "-", t = -t;
		int len = max(1, t.largestBitPos() + 1);
		for(int i = len-1; i >= 0; i --) o << t.test(i);
		return o;
	}
};
