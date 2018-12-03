//単位元-1を追加して特別扱いしてやればLCAもモノイドになる
//lcatに!LCA.cppのSchieberVishkinLCAなどを入れること。

int lca(int x, int y) {
	if(x == -1 || y == -1) return x == -1 ? y : x;
	return lcat.query(x, y);
}

struct RangeLCA {
	typedef int Val;
	int n;
	vector<Val> d;
	void init(const vector<Val> &v) {
		for(n = 1; n < (int)v.size(); n *= 2);
		d.assign(n * 2, -1);
		rep(i, v.size()) d[n+i] = v[i];
		for(int i = n-1; i >= 0; i --)
			d[i] = lca(d[i*2], d[i*2+1]);
	}
	//[l, r)
	Val query(int l, int r) const {
		Val m = -1;
		for(; l && l + (l&-l) <= r; l += l&-l)
			m = lca(m, d[(n+l) / (l&-l)]);
		for(; l < r; r -= r&-r)
			m = lca(m, d[(n+r) / (r&-r) - 1]);
		return m;
	}
};
