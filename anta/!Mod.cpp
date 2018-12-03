int inverse(long long a, const int MOD) {
	long long b = MOD, u = 1, v = 0;
	while(b) {
		long long t = a / b;
		a -= t * b; swap(a, b);
		u -= t * v; swap(u, v);
	}
	return (u % MOD + MOD) % MOD;
}
unsigned powmod(unsigned long long a, unsigned long long k, const unsigned MOD) {
	a %= MOD;
	unsigned long long r = MOD == 1 ? 0 : 1;
	while(k) {
		if(k & 1)
			(r *= a) %= MOD;
		(a *= a) %= MOD;
		k >>= 1;
	}
	return (unsigned)r;
}

inline unsigned long long modmult(unsigned long long x, unsigned long long y, unsigned long long MOD) {
	unsigned long long a = x, r = 0;
	while(y) {
		if(y & 1)
			if((r += a) >= MOD) r -= MOD;
		if((a += a) >= MOD) a -= MOD;
		y >>= 1;
	}
	return r;
}

//!INCLUDE: !MulDivLL.cpp
long long powmodll(long long a, unsigned long long k, const long long MOD) {
	a %= MOD;
	long long r = MOD == 1 ? 0 : 1;
	while(k) {
		if(k & 1)
			r = mulmodll(r, a, MOD);
		a = mulmodll(a, a, MOD);
		k >>= 1;
	}
	return r;
}


