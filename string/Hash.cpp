inline int add(int x, int y, int mod = inf) {
	x += y;
	if (x >= mod) x -= mod;
	return x;
}
inline int mul(int x, int y, int mod = inf) {
	return (x * 1LL * y) % mod;
}
struct Hash	// S[2...4] => 2*b^2 + 3*b + 4
{
	int base, mod;
	string s; int l;
	vi pp, hf;

	Hash(const string &s, int base = 101, int mod = 1000000009): base(base), mod(mod), s(s), l(s.length()), pp(l), hf(l)
	{
		pp[0] = 1;
		FOR(i, 1, l - 1)pp[i] = mul(pp[i - 1], base, mod);
		// rr = {19, 15, 12, 23, 4, 21, 3, 5, 2, 20, 6, 7, 16, 8, 25, 26, 11, 14, 1, 13, 18, 22, 24, 10, 17, 9};

		hf[0] = s[0] - 'a';
		FOR(i, 1, l - 1)hf[i] = add(mul(hf[i - 1], base, mod), s[i] - 'a', mod);

		// hr[l - 1] = rr[s[l - 1] - 'a'];
		// NFOR(i, l - 2, 0)hr[i] = add(mul(hr[i + 1], base, mod), rr[s[i] - 'a'], mod);
	}

	inline int get(int i, int j)
	{
		if (i == 0)return hf[j];
		if (i <= j)	return add(hf[j], mod - mul(hf[i - 1], pp[j - i + 1], mod), mod);
		// else if (i == l - 1) return hr[j];
		// else 	return add(hr[j], mod - mul(hr[i + 1], pp[i - j + 1], mod), mod);

	}
};
