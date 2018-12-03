struct DSU {
	vi p, r;
	DSU(int n):p(n),r(n){FOR(i, 0, n - 1)p[i] = i;}

	int get(int i) {
		if (p[i] != i)p[i] = get(p[i]);
		return p[i];
	}
	int connect(int i, int j) {
		int x = get(i), y = get(j);
		if (x == y) return 0;
		if (r[x] > r[y]) swap(x, y);
		p[x] = y;
		if (r[x] == r[y]) ++r[y];
		return 1;
	}
};