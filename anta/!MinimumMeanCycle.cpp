
template<typename T>T gcd(T x, T y){if(y==0)return x;else return gcd(y,x%y);}

typedef int Weight;
typedef long long Weight2;
const Weight InfWeight = INF;
struct Edge {
	int to;
	Weight w;
	Edge() { }
	Edge(int to_, Weight w_): to(to_), w(w_) { }
};

pair<Weight,Weight> minimum_mean_cycle(const vector<vector<Edge> > &g) {
	int n = g.size();
	vector<vector<Weight> > d(n+1, vector<Weight>(n, InfWeight));
	rep(i, n) d[0][i] = 0;
	rep(k, n) rep(i, n) {
		Weight x = d[k][i];
		if(x == InfWeight) continue;
		each(j, g[i]) {
			if(d[k+1][j->to] > x + j->w)
				d[k+1][j->to] = x + j->w;
		}
	}
	Weight a = 1, b = 0;
	rep(i, n) {
		Weight aa = 0, bb = InfWeight;
		Weight dn = d[n][i];
		if(dn == InfWeight) continue;
		rep(k, n) {
			Weight dk = d[k][i];
			if(dk == InfWeight) continue;
			if(bb == InfWeight || (Weight2)aa * (n - k) < (Weight2)(dn - dk) * bb)
				aa = dn - dk, bb = n - k;
		}
		if(bb == InfWeight) continue;
		if(b == 0 || (Weight2)a * bb > (Weight2)aa * b)
			a = aa, b = bb;
	}
	Weight gg = gcd(a, b);
	return make_pair(a / gg, b / gg);
}
