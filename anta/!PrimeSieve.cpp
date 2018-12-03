vector<bool> isprime;
vector<int> primes;
void sieve(int n){
	if((int)isprime.size() >= n+1) return;
	isprime.assign(n+1, true);
	isprime[0] = isprime[1] = false;
	int sqrtn = (int)(sqrt(n * 1.) + .5);
	for(int i = 2; i <= sqrtn; i ++) if(isprime[i]) {
		for(int j = i * i; j <= n; j += i)
			isprime[j] = false;
	}
	primes.clear();
	for(int i = 2; i <= n; i ++) if(isprime[i])
		primes.push_back(i);
}

typedef long long FactorsInt;
typedef vector<pair<FactorsInt,int> > Factors;
void primeFactors(FactorsInt x, Factors &out_v) {
	out_v.clear();
	int sqrtx = (int)(sqrt(x*1.) + 10.5);
	sieve(sqrtx);
	each(p, primes) {
		if(*p > sqrtx) break;
		if(x % *p == 0) {
			int t = 1;
			x /= *p;
			while(x % *p == 0) {
				t ++;
				x /= *p;
			}
			out_v.push_back(make_pair(*p, t));
		}
	}
	if(x != 1) out_v.push_back(make_pair(x, 1));
}

int countDivisors(const vector<pair<int,int> > &factors) {
	int r = 1;
	each(i, factors) r *= i->second + 1;
	return r;
}

void getDivisors(ll x, vector<FactorsInt> &out_v) {
	Factors fs;
	primeFactors(x, fs);
	out_v.assign(1, 1);
	rep(i, fs.size()) {
		for(int j = (int)out_v.size()-1; j >= 0; j --) {
			FactorsInt x = out_v[j];
			rep(k, fs[i].second) {
				x *= fs[i].first;
				out_v.push_back(x);
			}
		}
	}
	sort(all(out_v));
}

//factorsの演算
//一部未検証！
//場合によってはとても効率が悪いのでTLEに注意！(<http://koj.miz-miz.biz/view_code/101228>のように素数を座標圧縮したりすること)
template<typename Op> inline Factors combineFactors(Op op, int id, const Factors &a, const Factors &b) {
	Factors c;
	int an = a.size(), bn = b.size();
	int ai = 0, bi = 0;
	while(ai < an || bi < bn) {
		typename Factors::value_type::first_type p; int q;
		if(ai < an && (bi >= bn || a[ai].first < b[bi].first)) {
			p = a[ai].first, q = op(a[ai].second, id);
			ai ++;
		}else if(bi < bn && (ai >= an || a[ai].first > b[bi].first)) {
			p = b[bi].first, q = op(id, b[bi].second);
			bi ++;
		}else {
			p = a[ai].first, q = op(a[ai].second, b[bi].second);
			ai ++, bi ++;
		}
		if(q != 0) c.push_back(mp(p, q));
	}
	return c;
}

Factors gcdFactors(const Factors &a, const Factors &b) {
	return combineFactors<const int&(*)(const int&,const int&)>(std::min<int>, 0, a, b);
}
Factors lcmFactors(const Factors &a, const Factors &b) {
	return combineFactors<const int&(*)(const int&,const int&)>(std::max<int>, 0, a, b);
}
Factors productFactors(const Factors &a, const Factors &b) {
	return combineFactors(std::plus<int>(), 0, a, b);
}
Factors divideFactors(const Factors &a, const Factors &b) {
	return combineFactors(std::minus<int>(), 0, a, b);
}
