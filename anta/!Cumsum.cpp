struct Cumsum1 {
	typedef int T;
	typedef ll Sum;
	vector<Sum> v;
	void init(const vector<T> &a) {
		int n = a.size();
		v.assign(n+1, 0);
		for(int i = 1; i <= n; i ++) v[i] = v[i-1] + a[i-1];
	}
	inline Sum get(int l, int r) const { return v[r] - v[l]; }
};
struct CumsumLinear1 : Cumsum1 {
	void init(const vector<T> &a) {
		int n = a.size();
		vi b(n);
		for(int i = 0; i < n; i ++) b[i] = (Sum)a[i] * i;
		Cumsum1::init(b);
	}
};
