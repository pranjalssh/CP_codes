//!INCLUDE: !GCD-LCM.cpp
//!INCLUDE: !Mod.cpp
//未検証！
//a^n mod m = 1 となる最小のn＞0を求める。
//multiplicativeOrderPは素数の累乗 m = p^e

long long multiplicativeOrderP(long long a, long long p, int e) {
	long long m = 1;
	rep(i, e) m *= p;
	long long t = m / p * (p - 1);
	vector<FactorsInt> ds;
	getDivisors(t, ds);
	rep(i, ds.size()) {
		long long fac = 1;
		if(powmodll(a, ds[i], m) == 1)
			return ds[i];
	}
	return 0;
}

long long multiplicativeOrder(long long a, long long m) {
	Factors fs;
	primeFactors(m, fs);
	long long res = 1;
	rep(i, fs.size())
		res = lcm(res, multiplicativeOrderP(a, fs[i].first, fs[i].second));
	return res;
}
