//引き算は未検証！
//<http://koj.miz-miz.biz/view_code/101636>

template<typename T> T log1p(T x) {	//log1pが提供されてない場合に使う。glibcのコメントより
	T u = 1.0 + x;
	if(u == 1.0) return x;
	else return log(u) * (x / (u - 1.0));
}
template<typename T> T logaddexp(T x, T y) {
	T z = x - y;
	if(z > 0.0) return x + log1p(exp(-z));
	else if(z == z) return y + log1p(exp(z));
	else return x + y;
}
template<typename T> T logsubexp(T x, T y) {
	T z = y - x;
	if(z <= 0.0) return x + log1p(-exp(z));
	else if(z == z) return -numeric_limits<T>::infinity();	//とりあえず0
	else return x + y;
}
struct LogDomain {
	typedef long double T;
	T x;
	T get() const { return exp(x); }
	LogDomain(): x(-numeric_limits<T>::infinity()) { }
	LogDomain(T x_): x(log(x_)) { }
	static LogDomain fromlog(T x_) { LogDomain t; t.x = x_; return t; }
	LogDomain &operator*=(const LogDomain &that) { x += that.x; return *this; }
	LogDomain operator*(const LogDomain &that) const { return fromlog(x + that.x); }
	LogDomain &operator/=(const LogDomain &that) { x -= that.x; return *this; }
	LogDomain operator/(const LogDomain &that) const { return fromlog(x - that.x); }
	LogDomain operator^(const T &that) const { return fromlog(that == 0 ? T(0) : x * that); }
	LogDomain &operator+=(const LogDomain &that) { x = logaddexp(x, that.x); return *this; }
	LogDomain operator+(const LogDomain &that) const { return fromlog(logaddexp(x, that.x)); }
	LogDomain &operator-=(const LogDomain &that) { x = logsubexp(x, that.x); return *this; }
	LogDomain operator-(const LogDomain &that) const { return fromlog(logsubexp(x, that.x)); }
};
ostream &operator<<(ostream &o, LogDomain x) { return o << (double)x.get(); }

//lgammaとかも(C++11から)あるし、近似式もある。非推奨
vector<LogDomain> fact;
void compute_fact(int n) {
	fact.assign(n+1, LogDomain());
	fact[0] = 1;
	rer(i, 1, n) fact[i] = fact[i-1] * i;
}
LogDomain comb(int n, int r) {
	return n < r ? 0 : fact[n] / fact[n-r] / fact[r];
}
