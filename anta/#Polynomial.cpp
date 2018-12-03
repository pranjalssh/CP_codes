//未検証！

struct Polynomial {
	typedef mint Coef; typedef Coef Val;
	vector<Coef> coef;	//... + coef[2] x^2 + coef[1] x + coef[0]
	Polynomial() {}
	explicit Polynomial(int n): coef(n) {}
	static Polynomial One() {
		Polynomial r(1);
		r.coef[0] = 1;
		return r;
	}
	bool iszero() const { return coef.empty(); }
	int degree1() const { return coef.size(); }	//degree + 1
	int resize(int d) { if(degree1() < d) coef.resize(d); return d; }
	const Coef operator[](int i) const {
		return i >= degree1() ? Coef() : coef[i];
	}
	void canonicalize() {
		int i = coef.size();
		while(i > 0 && coef[i-1] == Coef()) i --;
		coef.resize(i);
	}
	Val evalute(Val x) const {
		int d = degree1();
		Val t = 0, y = 1;
		rep(i, d) {
			t += y * coef[i];
			y *= x;
		}
		return t;
	}
	Polynomial &operator+=(const Polynomial &that) {
		int d = resize(that.degree1());
		for(int i = 0; i < d; i ++) coef[i] += that[i];
		canonicalize();
		return *this;
	}
	Polynomial operator+(const Polynomial &that) const { return Polynomial(*this) += that; }
	Polynomial &operator-=(const Polynomial &that) {
		int d = resize(that.degree1());
		for(int i = 0; i < d; i ++) coef[i] -= that[i];
		canonicalize();
		return *this;
	}
	Polynomial operator-(const Polynomial &that) const { return Polynomial(*this) -= that; }
	//naive
	Polynomial operator*(const Polynomial &that) const {
		if(iszero() || that.iszero()) return Polynomial();
		int x = degree1(), y = that.degree1(), d = x + y - 1;
		Polynomial res(d);
		rep(i, x) rep(j, y)
			res.coef[i+j] += coef[i] * that.coef[j];
		res.canonicalize();
		return res;
	}
	//long division
	pair<Polynomial, Polynomial> divmod(const Polynomial &that) const {
		int x = degree1() - 1, y = that.degree1() - 1;
		int d = max(0, x - y);
		Polynomial q(d + 1), r = *this;
		for(int i = x; i >= y; i --) {
			Coef t = r.coef[i] / that.coef[y];
			q.coef[i - y] = t;
			r.coef[i] = 0;
			if(t == 0) continue;
			for(int j = 0; j < y; j ++)
				r.coef[i - y + j] -= t * that.coef[j];
		}
		q.canonicalize(); r.canonicalize();
		return make_pair(q, r);
	}
	Polynomial operator/(const Polynomial &that) const { return divmod(that).first; }
	Polynomial operator%(const Polynomial &that) const { return divmod(that).second; }

	static Polynomial interpolate(const vector<pair<Coef,Val> > &points) {
		int n = points.size();
		vector<Coef> dp(n+1);
		dp[0] = 1;
		rep(i, n) for(int j = i; j >= 0; j --) {
			dp[j+1] += dp[j];
			dp[j] *= -points[i].first;
		}
		Polynomial r(n);
		rep(i, n) {
			Coef den = 1;
			rep(j, n) if(i != j)
				den *= points[i].first - points[j].first;
			Coef iden = (Coef)1 / den, minus = 0;
			for(int j = n-1; j >= 0; j --) {
				minus = dp[j+1] + minus * points[i].first;
				r.coef[j] += minus * iden * points[i].second;
			}
		}
		r.canonicalize();
		return r;
	}
};
ostream &operator<<(ostream &o, const Polynomial &p) {
	bool first = true;
	for(int i = p.degree1() - 1; i >= 0; i --) {
		Polynomial::Coef c = p.coef[i];
		if(c != 0) {
			if(first && c < 0) o << "-";
			else if(first) o << "";
			else if(c < 0) o << " - ";
			else o << " + ";

			if(i != 0 && (c == 1 || c == -1)) o << "";
			else if(c < 0) o << -c;
			else o << c;

			if(i == 0) o << "";
			else if(i == 1) o << "x";
			else o << "x^" << i;

			first = false;
		}
	}
	if(first) o << "0";
	return o;
}

//根が全て整数のとき因数分解する
//https://judge.npca.jp/submissions/detail/3673
//で使っているやつ：
	bool is_root(Coef x) const {
		static const int ps[3] = {
			1000000007,1000000009,1000000021,
		};
		int d = degree1();
		for(int pi = 0; pi < 3; pi ++) {
			int p = ps[pi]; int xp;
			if((xp = x % p + p) >= p) xp -= p;
			long long t = 0, y = 1; int a;
			rep(i, d) {
				if((a = y * coef[i] % p + p) >= p) a -= p;
				if((t += a) >= p) t -= p;
				(y *= xp) %= p;
			}
			if(t != 0) return false;
		}
		return true;
	}
	void integral_root_test(vector<int> &out_roots) const {
		assert(degree1() >= 1 && coef[degree1()-1] == 1 && coef[0] != 0);
		Coef a0 = coef[0];
		vector<int> divisors;
		getDivisors(a0, divisors, true);
		out_roots.clear();
		each(d, divisors) if(is_root(*d))
			out_roots.push_back(*d);
	}

