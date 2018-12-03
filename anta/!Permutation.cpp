//vector ver. (未検証！)
struct Permutation {
	typedef int Index;
	int m;
	vector<Index> v;
	Permutation() {}
	Permutation(int m_): v(m), m(m_) {
		clear();
	}
	void clear() {
		rep(i, m) v[i] = i;
	}
	Permutation operator*(const Permutation &that) const {
		assert(m == that.m);
		Permutation x(m);
		rep(i, m) x.v[i] = that.v[v[i]];
		return x;
	}
	Permutation &operator*=(const Permutation &that) {
		return *this = *this * that;
	}
	Permutation inverse() const {
		Permutation x(m);
		rep(i, m) x.v[v[i]] = i;
		return x;
	}
};

//配列ver.
struct Permutation {
	typedef unsigned char Index;
	static const int MaxM = 6;
	int m;
	Index v[MaxM];
	Permutation() {}
	Permutation(int m_): m(m_) {
		clear();
	}
	void clear() {
		rep(i, m) v[i] = i;
	}
	Permutation operator*(const Permutation &that) const {
		assert(m == that.m);
		Permutation x(m);
		rep(i, m) x.v[i] = that.v[v[i]];
		return x;
	}
	Permutation &operator*=(const Permutation &that) {
		return *this = *this * that;
	}
	Permutation inverse() const {
		Permutation x(m);
		rep(i, m) x.v[v[i]] = i;
		return x;
	}
};
Permutation operator^(const Permutation& t, ll k) {
	Permutation A = t, B = Permutation(t.m);
	while(k) {
		if(k & 1) B *= A;
		A *= A;
		k >>= 1;
	}
	return B;
}
