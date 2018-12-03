struct RollingHash {
	const unsigned P = 1000000007U;
	vector<unsigned long long> pow, phash;
	template<typename T> void init(const T *s, int n, unsigned long long Q = 1) {
		pow.resize(n+1); phash.resize(n+1);
		pow[0] = Q; phash[0] = 0;
		rep(i, n) {
			pow[i+1] = pow[i] * P;
			phash[i+1] = s[i] + phash[i] * P;
		}
	}
	unsigned long long h(int i) const { return phash[i]; }
	unsigned long long h(int i, int j) const { return h(j) - h(i) * pow[j-i]; }
};
