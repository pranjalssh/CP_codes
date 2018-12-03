
int discrete_logrithm(int a, int b, int MOD) {
	int sqrtMOD = (int)ceil(sqrt(MOD*1.));
	map<int,int> s;
	int x = 1;
	rer(i, 0, sqrtMOD) {
		if(!s.count(x)) s[x] = i;
		x = (int)((ll)x * a % MOD);
	}
	int c = inverse(powmod(a, sqrtMOD, MOD), MOD);
	int y = b;
	rer(i, 0, sqrtMOD-1) {
		if(s.count(y))
			return i*sqrtMOD + s[y];
		y = (ll)y * c % MOD;
	}
	return -1;
}

